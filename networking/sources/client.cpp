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
        create_sing_up_req(nickname, password);
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

void Client::create_sign_in_req(const QString& nickname, const QString& password)
{
    QJsonObject j_obj;
    j_obj.insert(Protocol_keys::request, (int)Protocol_codes::Request_code::sign_in);
    j_obj.insert(Protocol_keys::nickname, nickname);
    j_obj.insert(Protocol_keys::password, password);
    QJsonDocument j_doc(j_obj);
    m_session->m_request = j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

void Client::create_sing_up_req(const QString& nickname, const QString& password)
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
