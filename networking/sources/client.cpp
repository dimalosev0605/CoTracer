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
    m_previous_contacts.push_back(m_user_validator.get_nickname());
}

void Client::connect_to_server()
{
    m_session = std::unique_ptr<Session>(new Session(m_ios, "192.168.1.140", 1234));
    m_session->m_socket.open(m_session->m_ep.protocol());
    m_session->m_socket.async_connect(m_session->m_ep, [this](const boost::system::error_code& ec)
    {
        if(ec.value() == 0) {
            set_is_connected(true);
            qDebug() << "Success connection!";
        } else {
            qDebug() << "Connection error!";
            set_is_connected(false);
        }
    });
}

bool Client::occupy_sock()
{
    std::lock_guard<std::mutex> lock(m_is_free_sock_mutex);
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
    std::lock_guard<std::mutex> lock(m_is_free_sock_mutex);
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
        set_is_connected(false);
        free_sock();
        connect_to_server();
    }
}

void Client::on_response_received(const boost::system::error_code& ec, size_t bytes_transferred)
{
    if(ec.value() == 0) {
        process_data(bytes_transferred);
    } else {
        set_is_connected(false);
        free_sock();
        connect_to_server();
    }
}

bool Client::get_is_connected()
{
    return m_is_connected;
}

void Client::set_is_connected(bool value)
{
    m_is_connected = value;
    emit is_connected_changed();
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
        return;
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
        return;
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
        return;
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
        return;
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
        return true;
    }
    return false;
}

void Client::fetch_14_days_stat()
{
    if(!get_is_connected()) {
        connect_to_server();
        return;
    }
    if(occupy_sock()) {
        create_fetch_14_days_stat_req();
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
    fetch_contacts_based_on_date(date);
    return new_model;
}

Contacts_model* Client::create_model_based_on_nickname(const QString& nickname, const QString& date)
{
    Contacts_model* new_model = new Contacts_model(m_previous_contacts, this);

    connect(this, &Client::contacts_received, new_model, &Contacts_model::receive_contacts,
            Qt::QueuedConnection);
    m_models.push_back(new_model);
    fetch_contacts_based_on_nickname(nickname, date);
    return new_model;
}

void Client::pop_model()
{
    if(!m_models.isEmpty()) {
        auto poped_model = m_models.back();
        m_models.pop_back();
        delete poped_model;

        if(!m_previous_contacts_count.isEmpty()) {
            int must_poped = m_previous_contacts_count.back();
            m_previous_contacts_count.pop_back();
            for(int i = 0; i < must_poped; ++i) {
                m_previous_contacts.pop_back();
            }
        }
    }
}

void Client::create_sign_in_req(const QString& nickname, const QString& password)
{
    QJsonObject j_obj;
    j_obj.insert(Protocol_keys::request, (int)Protocol_codes::Request_code::sign_in);
    j_obj.insert(Protocol_keys::nickname, nickname);
    j_obj.insert(Protocol_keys::password, password);
    QJsonDocument j_doc(j_obj);
    m_session->m_request = j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

void Client::create_sign_up_req(const QString& nickname, const QString& password)
{
    QJsonObject j_obj;
    j_obj.insert(Protocol_keys::request, (int)Protocol_codes::Request_code::sign_up);
    j_obj.insert(Protocol_keys::nickname, nickname);
    j_obj.insert(Protocol_keys::password, password);
    QJsonDocument j_doc(j_obj);
    m_session->m_request = j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

void Client::create_change_password_req(const QString& new_password)
{
    QJsonObject j_obj;
    j_obj.insert(Protocol_keys::request, (int)Protocol_codes::Request_code::change_password);
    j_obj.insert(Protocol_keys::nickname, m_user_validator.get_nickname());
    j_obj.insert(Protocol_keys::password, new_password);
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

    j_obj.insert(Protocol_keys::request, (int)Protocol_codes::Request_code::change_avatar);
    j_obj.insert(Protocol_keys::nickname, m_user_validator.get_nickname());

    QFile avatar_file(avatar_path);
    QByteArray base64_img;
    if(avatar_file.open(QIODevice::ReadOnly)) {
        QByteArray b_arr = avatar_file.readAll();
        base64_img = b_arr.toBase64();
    }
    if(base64_img.isEmpty()) return false;

    j_obj.insert(Protocol_keys::avatar, QString::fromLatin1(base64_img));

    QJsonDocument j_doc(j_obj);
    m_session->m_request = j_doc.toJson().append(Protocol_keys::end_of_message).data();

    return true;
}

void Client::create_fetch_14_days_stat_req()
{
    QJsonObject j_obj;
    j_obj.insert(Protocol_keys::request, (int)Protocol_codes::Request_code::stats_for_14_days);
    j_obj.insert(Protocol_keys::nickname, m_user_validator.get_nickname());
    QJsonDocument j_doc(j_obj);
    m_session->m_request = j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

void Client::create_fetch_contacts_based_on_date_req(const QString& date)
{
    QJsonObject j_obj;
    j_obj.insert(Protocol_keys::request, (int)Protocol_codes::Request_code::get_contacts);
    j_obj.insert(Protocol_keys::nickname, m_user_validator.get_nickname());
    j_obj.insert(Protocol_keys::contact_date, date);
    QJsonDocument j_doc(j_obj);
    m_session->m_request = j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

void Client::create_fetch_contacts_based_on_nickname_req(const QString& nickname, const QString& date)
{
    QJsonObject j_obj;
    j_obj.insert(Protocol_keys::request, (int)Protocol_codes::Request_code::get_contacts);
    j_obj.insert(Protocol_keys::nickname, nickname);
    j_obj.insert(Protocol_keys::contact_date, date);
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

        Protocol_codes::Response_code res_code = (Protocol_codes::Response_code)j_map[Protocol_keys::response].toInt();

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
            m_user_validator.reset_user_avatar();
            break;
        }
        case Protocol_codes::Response_code::success_fetching_stats_for_14_days: {
            process_success_fetching_stat_for_14_days(j_map);
            break;
        }
        case Protocol_codes::Response_code::contacts_list: {
            process_contacts_list(j_map);
            break;
        }
        case Protocol_codes::Response_code::internal_server_error: {
            break;
        }

        }
    }

    free_sock();
}

void Client::process_success_sign_in(QMap<QString, QVariant>& j_map)
{
    QString avatar_str = j_map[Protocol_keys::avatar].toString();
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

void Client::process_success_fetching_stat_for_14_days(QMap<QString, QVariant>& j_map)
{
    auto j_arr_of_stats = j_map[Protocol_keys::statistics_for_14_days].toJsonArray();

    QVector<std::tuple<QString, int, int>> stats;

    for(int i = 0; i < j_arr_of_stats.size(); ++i) {
        auto j_stat_for_day = j_arr_of_stats[i].toObject();
        auto day_map = j_stat_for_day.toVariantMap();

        QString date = day_map[Protocol_keys::date].toString();
        int unreg_qnt = day_map[Protocol_keys::unregisterd_quantity].toInt();
        int reg_qnt = day_map[Protocol_keys::registered_quantity].toInt();

        stats.push_back(std::make_tuple(date, reg_qnt, unreg_qnt));
    }
    emit statistic_received(stats);
}

void Client::process_contacts_list(QMap<QString, QVariant>& j_map)
{
    QJsonArray j_arr_of_reg_contacts = j_map[Protocol_keys::registered_list].toJsonArray();

    QVector<std::tuple<QString, QString, bool>> received_contacts;

    for(int i = 0; i < j_arr_of_reg_contacts.size(); ++i) {
        auto contact_j_obj = j_arr_of_reg_contacts[i].toObject();
        auto contact_j_obj_map = contact_j_obj.toVariantMap();

        auto pair = std::make_pair(contact_j_obj_map[Protocol_keys::nickname].toString(),
                                   contact_j_obj_map[Protocol_keys::contact_time].toString());

        received_contacts.push_back(std::make_tuple(pair.first, pair.second, true));
        m_previous_contacts.push_back(pair.first);
    }
    m_previous_contacts_count.push_back(j_arr_of_reg_contacts.size());

    // begin parse array of avatars
    QDir dir_with_avatars(m_path_finder.get_path_to_avatars_dir());

    QJsonArray avatars = j_map[Protocol_keys::avatars].toJsonArray();

    for(int i = 0; i < avatars.size(); ++i) {
        auto avatar_obj = avatars[i].toObject();
        auto map = avatar_obj.toVariantMap();

        QString nickname = map[Protocol_keys::nickname].toString();

        QString avatar_str = map[Protocol_keys::avatar].toString();
        QByteArray avatar = QByteArray::fromBase64(avatar_str.toLatin1());

        QString avatar_path(dir_with_avatars.absolutePath() + '/' + nickname);
        QFile file(avatar_path);

        if(file.open(QIODevice::WriteOnly)) {
            file.write(avatar);
        }
    }
    // end parse array of avatars

    QJsonArray j_arr_of_unreg_contacts = j_map[Protocol_keys::unregistered_list].toJsonArray();

    for(int i = 0; i < j_arr_of_unreg_contacts.size(); ++i) {
        auto contact_j_obj = j_arr_of_unreg_contacts[i].toObject();
        auto contact_j_obj_map = contact_j_obj.toVariantMap();

        auto pair = std::make_pair(contact_j_obj_map[Protocol_keys::nickname].toString(),
                                   contact_j_obj_map[Protocol_keys::contact_time].toString());

        received_contacts.push_back(std::make_tuple(pair.first, pair.second, false));
    }

    emit contacts_received(received_contacts);
    disconnect(this, &Client::contacts_received, nullptr, nullptr);
}

void Client::fetch_contacts_based_on_date(const QString& date)
{
    if(!get_is_connected()) {
        connect_to_server();
        return;
    }
    if(occupy_sock()) {
        create_fetch_contacts_based_on_date_req(date);
        async_write();
    }
}

void Client::fetch_contacts_based_on_nickname(const QString &nickname, const QString& date)
{
    if(!get_is_connected()) {
        connect_to_server();
        return;
    }
    if(occupy_sock()) {
        create_fetch_contacts_based_on_nickname_req(nickname, date);
        async_write();
    }
}
