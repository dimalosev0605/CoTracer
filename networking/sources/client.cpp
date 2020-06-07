#include "./networking/headers/client.h"

Client::Client(QObject *parent)
    : QObject(parent),
      m_work(boost::asio::make_work_guard(m_ios))
{
    m_thread.reset(new std::thread([this]()
    {
        connect_to_server();
        m_ios.run();
    }));
    m_thread->detach();

    set_is_authorized(m_user_validator.get_is_authorized());
    read_cached_avatars_info_file();
}

Client::~Client()
{
    qDebug() << this << " destroyed!";
    save_cached_avatars_info_file();
}

void Client::connect_to_server()
{
    {
        std::lock_guard<std::mutex> lock(m_is_sock_free_mutex);
        m_is_sock_free = false;
    }

    m_session = std::unique_ptr<Session>(new Session(m_ios, "192.168.1.140", 1234));
    m_session->m_socket.open(m_session->m_ep.protocol());
    m_session->m_socket.async_connect(m_session->m_ep, [this](const boost::system::error_code& ec)
    {
        if(ec.value() == 0) {
            set_is_connected(true);
            qDebug() << "Success connection!";
        } else {
            set_is_connected(false);
            qDebug() << "Connection error!";
        }
        free_sock();
    });
}

bool Client::occupy_sock()
{
    std::lock_guard<std::mutex> lock(m_is_sock_free_mutex);
    if(m_is_sock_free) {
        m_is_sock_free = false;
        return true;
    }
    else {
        return false;
    }
}

void Client::free_sock()
{
    std::lock_guard<std::mutex> lock(m_is_sock_free_mutex);
    m_is_sock_free = true;
}

void Client::async_write()
{
    boost::asio::async_write(m_session->m_socket, boost::asio::buffer(m_session->m_request),
                             boost::bind
                             (&Client::on_request_sent,
                              boost::ref(*this),
                              boost::placeholders::_1,
                              boost::placeholders::_2)
                             );
}

void Client::on_request_sent(const boost::system::error_code& ec, size_t bytes_transferred)
{
    m_session->m_request.clear();
    if(ec.value() == 0) {
        boost::asio::async_read_until(m_session->m_socket, m_session->m_response, Protocol_keys::end_of_message.toStdString(),
                                      boost::bind
                                      (&Client::on_response_received,
                                       boost::ref(*this),
                                       boost::placeholders::_1,
                                       boost::placeholders::_2)
                                      );
    } else {
        if(ec == boost::asio::error::operation_aborted) {
            qDebug() << "Operation canceled";
            free_sock();
            set_is_connected(false);
            connect_to_server();
            return;
        }
        free_sock();
        set_is_connected(false);
        connect_to_server();
    }
}

void Client::on_response_received(const boost::system::error_code& ec, size_t bytes_transferred)
{
    if(ec.value() == 0) {
        process_data(bytes_transferred);
    } else {
        if(ec == boost::asio::error::operation_aborted) {
            qDebug() << "Operation canceled";
            free_sock();
            set_is_connected(false);
            connect_to_server();
            return;
        }
        free_sock();
        set_is_connected(false);
        connect_to_server();
    }
}

bool Client::get_is_connected()
{
    std::lock_guard<std::mutex> lock(m_is_connected_mutex);
    return m_is_connected;
}

void Client::set_is_connected(bool value)
{
    std::lock_guard<std::mutex> lock(m_is_connected_mutex);
    m_is_connected = value;
}

bool Client::get_is_authorized()
{
    return m_is_authorized;
}

void Client::set_is_authorized(bool value)
{
    m_is_authorized = value;
    emit is_authorized_changed();
}

void Client::sign_in(const QString& nickname, const QString& password)
{
    if(!get_is_connected()) {
        connect_to_server();
    }
    if(occupy_sock()) {
        m_user_validator.set_nickname(nickname);
        m_user_validator.set_password(password);
        create_sign_in_req(nickname, password);
        async_write();
    }
}

void Client::sign_up(const QString& nickname, const QString& password)
{
    if(!get_is_connected()) {
        connect_to_server();
    }
    if(occupy_sock()) {
        m_user_validator.set_nickname(nickname);
        m_user_validator.set_password(password);
        create_sign_up_req(nickname, password);
        async_write();
    }
}

void Client::change_password(const QString& new_password)
{
    if(!get_is_connected()) {
        connect_to_server();
    }
    if(occupy_sock()) {
        create_change_password_req(new_password);
        async_write();
    }
}

void Client::change_avatar(const QString& new_avatar_path)
{
    if(!get_is_connected()) {
        connect_to_server();
    }
    if(occupy_sock()) {
        if(create_change_avatar_req(new_avatar_path)) {
            async_write();
        }
        else {
            free_sock();
        }
    }
}

bool Client::exit_from_account()
{
    if(m_user_validator.exit_from_account()) {
        set_is_authorized(false);
        m_cached_avatars.clear();
        return true;
    }
    return false;
}

void Client::fetch_stat_for_14_days()
{
    if(!get_is_connected()) {
        connect_to_server();
    }
    if(occupy_sock()) {
        create_fetch_stat_for_14_days_req();
        async_write();
    }
}

void Client::add_contact(const QString& nickname, const QString& time, const QString& date)
{
    if(nickname == m_user_validator.get_nickname()) return;
    if(!get_is_connected()) {
        connect_to_server();
    }
    if(occupy_sock()) {
        create_add_contact_req(nickname, time, date);

        // lol
        lol_vector.push_back(QString(nickname));
        lol_vector.push_back(QString(time));
        // lol

        async_write();
    }
}

void Client::remove_contact(const QString& nickname, const QString& time, const QString& date, int index_in_table)
{
    if(!get_is_connected()) {
        connect_to_server();
    }
    if(occupy_sock()) {
        create_remove_contact_req(nickname, time, date);

        // lol
        lol_vector.push_back((int)index_in_table);
        // lol

        async_write();
    }
}

Contacts_model* Client::create_model_based_on_date(const QString& date)
{
    Contacts_model* new_model = new Contacts_model(this);

    connect(this, &Client::contacts_received, new_model, &Contacts_model::receive_contacts,
            Qt::QueuedConnection);
    connect(this, &Client::success_contact_deletion, new_model, &Contacts_model::remove_contact,
            Qt::QueuedConnection);
    connect(this, &Client::success_contact_adding, new_model, &Contacts_model::add_contact,
            Qt::QueuedConnection);

    m_models.push_back(new_model);
    fetch_contacts(m_user_validator.get_nickname(), date);
    return new_model;
}

Contacts_model* Client::create_model_based_on_date_and_nickname(const QString& nickname, const QString& date)
{
    Contacts_model* new_model = new Contacts_model(this);

    connect(this, &Client::contacts_received, new_model, &Contacts_model::receive_contacts,
            Qt::QueuedConnection);

    m_models.push_back(new_model);
    fetch_contacts(nickname, date);
    return new_model;
}

void Client::pop_model()
{
    if(!m_models.isEmpty()) {
        auto poped_model = m_models.back();
        m_models.pop_back();
        delete poped_model;
    }
}

void Client::cancel_operation()
{
    m_session->m_socket.cancel();
    if(!get_is_connected()) {
        connect_to_server();
    }
}

QString Client::get_nickname() const
{
    return m_user_validator.get_nickname();
}

QString Client::get_password() const
{
    return m_user_validator.get_password();
}

QString Client::get_avatar_path(bool prefix) const
{
    QFile check_existence(m_path_finder.get_path_to_user_avatar(false));
    if(check_existence.exists()) {
        return m_path_finder.get_path_to_user_avatar(prefix);
    }
    else {
        return m_path_finder.get_path_to_default_avatar_path();
    }
}

void Client::create_sign_in_req(const QString& nickname, const QString& password)
{
    QJsonObject j_obj;
    j_obj.insert(Protocol_keys::request_code, (int)Protocol_codes::Request_code::sign_in);
    j_obj.insert(Protocol_keys::user_nickname, nickname);
    j_obj.insert(Protocol_keys::user_password, password);
    QJsonDocument j_doc(j_obj);
    m_session->m_request = j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

void Client::create_sign_up_req(const QString& nickname, const QString& password)
{
    QJsonObject j_obj;
    j_obj.insert(Protocol_keys::request_code, (int)Protocol_codes::Request_code::sign_up);
    j_obj.insert(Protocol_keys::user_nickname, nickname);
    j_obj.insert(Protocol_keys::user_password, password);
    QJsonDocument j_doc(j_obj);
    m_session->m_request = j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

void Client::create_change_password_req(const QString& new_password)
{
    QJsonObject j_obj;
    j_obj.insert(Protocol_keys::request_code, (int)Protocol_codes::Request_code::change_password);
    j_obj.insert(Protocol_keys::user_nickname, m_user_validator.get_nickname());
    j_obj.insert(Protocol_keys::user_password, new_password);
    m_user_validator.set_new_password(new_password);
    QJsonDocument j_doc(j_obj);
    m_session->m_request = j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

bool Client::create_change_avatar_req(const QString& new_avatar_path)
{
    QString avatar_path = new_avatar_path;
    avatar_path.remove("file://");
    m_user_validator.set_new_avatar_path(avatar_path);

    QJsonObject j_obj;
    j_obj.insert(Protocol_keys::request_code, (int)Protocol_codes::Request_code::change_avatar);
    j_obj.insert(Protocol_keys::user_nickname, m_user_validator.get_nickname());

    QFile avatar_file(avatar_path);
    QByteArray base64_img;
    if(avatar_file.open(QIODevice::ReadOnly)) {
        QByteArray b_arr = avatar_file.readAll();
        base64_img = b_arr.toBase64();
    }
    if(base64_img.isEmpty()) return false;

    j_obj.insert(Protocol_keys::avatar_data, QString::fromLatin1(base64_img));

    QJsonDocument j_doc(j_obj);
    m_session->m_request = j_doc.toJson().append(Protocol_keys::end_of_message).data();

    return true;
}

void Client::create_fetch_stat_for_14_days_req()
{
    QJsonObject j_obj;
    j_obj.insert(Protocol_keys::request_code, (int)Protocol_codes::Request_code::fetch_stat_for_14_days);
    j_obj.insert(Protocol_keys::user_nickname, m_user_validator.get_nickname());
    QJsonDocument j_doc(j_obj);
    m_session->m_request = j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

void Client::create_fetch_contacts_req(const QString& nickname, const QString& date)
{
    QJsonObject j_obj;
    j_obj.insert(Protocol_keys::request_code, (int)Protocol_codes::Request_code::fetch_contacts);
    j_obj.insert(Protocol_keys::contact_nickname, nickname);
    j_obj.insert(Protocol_keys::contact_date, date);
    insert_info_about_cached_avatars_in_req(j_obj);
    QJsonDocument j_doc(j_obj);
    m_session->m_request = j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

void Client::create_add_contact_req(const QString& nickname, const QString& time, const QString& date)
{
    QJsonObject j_obj;
    j_obj.insert(Protocol_keys::request_code, (int)Protocol_codes::Request_code::add_contact);
    j_obj.insert(Protocol_keys::user_nickname, m_user_validator.get_nickname());
    j_obj.insert(Protocol_keys::contact_nickname, nickname);
    j_obj.insert(Protocol_keys::contact_date, date);
    j_obj.insert(Protocol_keys::contact_time, time);
    QJsonDocument j_doc(j_obj);
    m_session->m_request = j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

void Client::create_remove_contact_req(const QString& nickname, const QString& time, const QString& date)
{
    QJsonObject j_obj;
    j_obj.insert(Protocol_keys::request_code, (int)Protocol_codes::Request_code::remove_contact);
    j_obj.insert(Protocol_keys::user_nickname, m_user_validator.get_nickname());
    j_obj.insert(Protocol_keys::contact_date, date);
    j_obj.insert(Protocol_keys::contact_nickname, nickname);
    j_obj.insert(Protocol_keys::contact_time, time);
    QJsonDocument j_doc(j_obj);
    m_session->m_request = j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

void Client::process_data(std::size_t bytes_transferred)
{
    std::string data((const char*)m_session->m_response.data().data(), bytes_transferred - Protocol_keys::end_of_message.size());
    auto j_doc = QJsonDocument::fromJson(data.c_str());
    m_session->m_response.consume(bytes_transferred);

    if(!j_doc.isEmpty()) {
        auto j_obj = j_doc.object();
        auto j_map = j_obj.toVariantMap();

        Protocol_codes::Response_code res_code = (Protocol_codes::Response_code)j_map[Protocol_keys::response_code].toInt();

        switch (res_code) {

        case Protocol_codes::Response_code::success_sign_in: {
            process_success_sign_in(j_map);
            break;
        }
        case Protocol_codes::Response_code::success_sign_up: {
            process_success_sign_up();
            break;
        }
        case Protocol_codes::Response_code::success_password_changing: {
            process_success_password_changing();
            break;
        }
        case Protocol_codes::Response_code::success_avatar_changing: {
            process_success_avatar_changing();
            break;
        }
        case Protocol_codes::Response_code::success_fetching_stat_for_14_days: {
            process_success_fetching_stat_for_14_days(j_map);
            break;
        }
        case Protocol_codes::Response_code::success_fetching_contacts: {
            process_success_fetching_contacts(j_map);
            break;
        }
        case Protocol_codes::Response_code::success_contact_adding: {
            process_success_contact_adding();
            break;
        }
        case Protocol_codes::Response_code::such_contact_not_exists: {
            process_such_contact_not_exists();
            break;
        }
        case Protocol_codes::Response_code::success_contact_deletion: {
            process_success_contact_deletion();
            break;
        }
        case Protocol_codes::Response_code::internal_server_error: {
            process_internal_server_error();
            break;
        }

        }
    }

    free_sock();
}

void Client::process_success_sign_in(QMap<QString, QVariant>& j_map)
{
    QString avatar_str = j_map[Protocol_keys::avatar_data].toString();
    QByteArray avatar_byte_arr = QByteArray::fromBase64(avatar_str.toLatin1());

    if(m_user_validator.save_user_info() && m_user_validator.save_user_avatar(avatar_byte_arr)) {
        set_is_authorized(true);
    }
}

void Client::process_success_sign_up()
{
    if(m_user_validator.save_user_info()) {
        set_is_authorized(true);
    }
}

void Client::process_success_password_changing()
{
    m_user_validator.set_password(m_user_validator.get_new_password());
    m_user_validator.save_user_info();
    emit update_password_field();
}

void Client::process_success_avatar_changing()
{
    m_user_validator.reset_user_avatar();
}

void Client::process_success_fetching_stat_for_14_days(QMap<QString, QVariant>& j_map)
{
    auto j_arr_of_stats = j_map[Protocol_keys::statistic_for_14_days].toJsonArray();

    QVector<std::tuple<QString, int>> statistic;

    for(int i = 0; i < j_arr_of_stats.size(); ++i) {
        auto j_stat_for_day = j_arr_of_stats[i].toObject();
        auto day_map = j_stat_for_day.toVariantMap();

        QString date = day_map[Protocol_keys::stat_date].toString();
        int reg_qnt = day_map[Protocol_keys::quantity_of_contacts].toInt();

        statistic.push_back(std::make_tuple(date, reg_qnt));
    }
    emit statistic_received(statistic);
}

void Client::process_success_fetching_contacts(QMap<QString, QVariant>& j_map)
{
    QJsonArray j_arr_of_contacts = j_map[Protocol_keys::contact_list].toJsonArray();
    QVector<std::tuple<QString, QString>> received_contacts;
    QDir dir_with_avatars(m_path_finder.get_path_to_avatars_dir());

    for(int i = 0; i < j_arr_of_contacts.size(); ++i) {
        auto contact_j_obj = j_arr_of_contacts[i].toObject();
        auto contact_j_obj_map = contact_j_obj.toVariantMap();

        auto pair = std::make_pair(contact_j_obj_map[Protocol_keys::contact_nickname].toString(),
                                   contact_j_obj_map[Protocol_keys::contact_time].toString());

        received_contacts.push_back(std::make_tuple(pair.first, pair.second));

        QString avatar_str = contact_j_obj_map[Protocol_keys::avatar_data].toString();
        if(avatar_str.isEmpty()) continue;

        // if we receive avatar data -> save avatar
        QByteArray avatar = QByteArray::fromBase64(avatar_str.toLatin1());

        QString avatar_path(dir_with_avatars.absolutePath() + '/' + pair.first);
        QFile file(avatar_path);

        if(file.open(QIODevice::WriteOnly)) {
            file.write(avatar);
        }
        // end if we receive avatar data -> save avatar

        // insert new downloaded time in cached_avatars
        QString avatar_downloaded_date_time = contact_j_obj_map[Protocol_keys::avatar_downloaded_date_time].toString();
        auto iter = std::find_if(m_cached_avatars.begin(), m_cached_avatars.end(), [&](const std::tuple<QString, QString>& i)
        {
            return pair.first == std::get<0>(i);
        });
        if(iter != m_cached_avatars.end()) {
            auto updated_cached_obj = std::make_tuple(pair.first, avatar_downloaded_date_time);
            *iter = updated_cached_obj;
        }
        else {
            m_cached_avatars.push_back(std::make_tuple(pair.first, avatar_downloaded_date_time));
        }
        // end insert new downloaded time in cached_avatars
    }

    emit contacts_received(received_contacts);
    disconnect(this, &Client::contacts_received, nullptr, nullptr);
}

void Client::process_success_contact_adding()
{
    QString nickname = std::any_cast<QString>(lol_vector[0]);
    QString time = std::any_cast<QString>(lol_vector[1]);
    lol_vector.clear();
    emit success_contact_adding(nickname, time);
}

void Client::process_such_contact_not_exists()
{
    lol_vector.clear();
}

void Client::process_success_contact_deletion()
{
    int index = std::any_cast<int>(lol_vector[0]);
    lol_vector.clear();
    emit success_contact_deletion(index);
}

void Client::process_internal_server_error()
{
    lol_vector.clear();
}

void Client::fetch_contacts(const QString& nickname, const QString& date)
{
    if(!get_is_connected()) {
        connect_to_server();
    }
    if(occupy_sock()) {
        create_fetch_contacts_req(nickname, date);
        async_write();
    }
}

void Client::insert_info_about_cached_avatars_in_req(QJsonObject& j_obj)
{
    QJsonArray arr;
    for(size_t i = 0; i < m_cached_avatars.size(); ++i) {
        QJsonObject obj;
        obj.insert(Protocol_keys::contact_nickname, std::get<0>(m_cached_avatars[i]));
        obj.insert(Protocol_keys::avatar_downloaded_date_time, std::get<1>(m_cached_avatars[i]));
        arr.append(obj);
    }
    j_obj.insert(Protocol_keys::cached_avatars ,arr);
}

void Client::read_cached_avatars_info_file()
{
    m_cached_avatars.clear();

    QFile cached_avatars_file(m_path_finder.get_path_to_cached_avatars_info_file());
    if(cached_avatars_file.open(QIODevice::ReadOnly)) {
        auto doc = QJsonDocument::fromJson(cached_avatars_file.readAll());
        if(!doc.isEmpty()) {
            auto obj = doc.object();
            auto map = obj.toVariantMap();
            auto cached_avatars = map[Protocol_keys::cached_avatars].toJsonArray();
            for(int i = 0; i < cached_avatars.size(); ++i) {
                auto cached_obj = cached_avatars[i].toObject();
                QString contact_nickname = cached_obj[Protocol_keys::contact_nickname].toString();
                QString avatar_downloaded_date_time = cached_obj[Protocol_keys::avatar_downloaded_date_time].toString();
                m_cached_avatars.push_back(std::make_tuple(contact_nickname, avatar_downloaded_date_time));
            }
        }
    }
}

void Client::save_cached_avatars_info_file()
{
    QFile cached_avatars_info_file(m_path_finder.get_path_to_cached_avatars_info_file());
    if(cached_avatars_info_file.open(QIODevice::WriteOnly)) {
        QJsonArray arr;
        for(size_t i = 0; i < m_cached_avatars.size(); ++i) {
            QJsonObject cached_obj;
            cached_obj.insert(Protocol_keys::contact_nickname, std::get<0>(m_cached_avatars[i]));
            cached_obj.insert(Protocol_keys::avatar_downloaded_date_time, std::get<1>(m_cached_avatars[i]));
            arr.append(cached_obj);
        }
        QJsonObject obj;
        obj.insert(Protocol_keys::cached_avatars, arr);
        QJsonDocument doc(obj);
        cached_avatars_info_file.write(doc.toJson());
    }
}
