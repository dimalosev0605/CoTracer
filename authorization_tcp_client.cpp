#include "authorization_tcp_client.h"

Authorization_tcp_client::Authorization_tcp_client(QObject* parent)
    : Base_tcp_client(parent)
{
}

Authorization_tcp_client::~Authorization_tcp_client()
{
}

void Authorization_tcp_client::async_write()
{
    boost::asio::async_write(m_session->m_socket, boost::asio::buffer(m_session->m_request),
                             boost::bind
                             (&Authorization_tcp_client::on_request_sent,
                              boost::ref(*this),
                              boost::placeholders::_1,
                              boost::placeholders::_2)
                             );
}

void Authorization_tcp_client::sign_in(const QString& nickname, const QString& password)
{
    if(!get_is_connected()) {
        emit create_dialog("Connection error.", -2, false, false, false);
    }
    if(occupy()) {
        emit create_dialog("Please wait", -2, true, false, false);
        m_user_validator.set_nickname(nickname);
        m_user_validator.set_password(password);
        m_session->m_request = create_request(Protocol_codes::Request_code::sign_in, nickname, password);
        async_write();
    }
}

void Authorization_tcp_client::sign_up(const QString& nickname, const QString& password)
{
    if(!get_is_connected()) {
        emit create_dialog("Connection error.", -2, false, false, false);
    }
    if(occupy()) {
        emit create_dialog("Please wait", -2, true, false, false);
        m_user_validator.set_nickname(nickname);
        m_user_validator.set_password(password);
        m_session->m_request = create_request(Protocol_codes::Request_code::sign_up, nickname, password);
        async_write();
    }
}

bool Authorization_tcp_client::exit_from_account()
{
    if(m_user_validator.exit_from_account()) {
        set_is_authenticated(false);
        return true;
    }
    return false;
}

const char* Authorization_tcp_client::create_request(Protocol_codes::Request_code code, const QString& nickname, const QString& password)
{
    QJsonObject j_obj;

    j_obj.insert(Protocol_keys::request, (int)code);
    j_obj.insert(Protocol_keys::nickname, nickname);
    j_obj.insert(Protocol_keys::password, password);

    QJsonDocument j_doc(j_obj);
    return j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

void Authorization_tcp_client::parse_response(std::size_t bytes_transferred)
{
    std::string data((const char*)m_session->m_response.data().data(), bytes_transferred - Protocol_keys::end_of_message.size());
    auto j_doc = QJsonDocument::fromJson(data.c_str());
    m_session->m_response.consume(bytes_transferred);

    if(!j_doc.isEmpty()) {
        auto j_obj = j_doc.object();
        auto j_map = j_obj.toVariantMap();
        m_session->m_res_code = (Protocol_codes::Response_code)j_map[Protocol_keys::response].toInt();

        if(m_session->m_res_code == Protocol_codes::Response_code::success_sign_in) {
            parse_avatar(j_map);
        }
    }
}

void Authorization_tcp_client::parse_avatar(QMap<QString, QVariant>& j_map)
{
    QString str_avatar = j_map[Protocol_keys::avatar].toString();
    m_avatar = QByteArray::fromBase64(str_avatar.toLatin1());
}

void Authorization_tcp_client::process_data()
{
    switch (m_session->m_res_code) {

    case Protocol_codes::Response_code::success_sign_up: {
        if(m_user_validator.save_user_info()) {
            set_is_authenticated(true);
            emit change_dialog("Success sign up!", 2000, false, true, true);
        }
        else {
            emit change_dialog("File system error", 2000, false, true, true);
        }
        break;
    }
    case Protocol_codes::Response_code::sign_up_failure: {
        emit change_dialog("Such user already exists", 2000, false, true, true);
        break;
    }
    case Protocol_codes::Response_code::success_sign_in: {
        if(m_user_validator.save_user_info() && m_user_validator.save_user_avatar(m_avatar)) {
            m_avatar.clear();
            set_is_authenticated(true);
            emit change_dialog("Success sign in!", 2000, false, true, true);
        }
        else {
            emit change_dialog("File system error", 2000, false, true, true);
        }
        break;
    }
    case Protocol_codes::Response_code::sign_in_failure: {
        emit change_dialog("Incorrect nickname or password.", 2000, false, true, true);
        break;
    }
    case Protocol_codes::Response_code::internal_server_error: {
        emit change_dialog("Internal server error", 2000, false, true, true);
        break;
    }
    case Protocol_codes::Response_code::success_avatar_changing: {
        m_user_validator.save_avatar(m_avatar_path);
        emit change_dialog("Avatar was successfully changed!", 2000, false, true, true);
        break;
    }

    }
    release();
}

void Authorization_tcp_client::on_request_sent(const boost::system::error_code& ec, size_t bytes_transferred)
{
    m_session->m_request.clear();
    if(ec.value() == 0) {
        boost::asio::async_read_until(m_session->m_socket, m_session->m_response, Protocol_keys::end_of_message.toStdString(),
                                      boost::bind
                                      (&Authorization_tcp_client::on_response_received,
                                       boost::ref(*this),
                                       boost::placeholders::_1,
                                       boost::placeholders::_2)
                                      );
    } else {
        set_is_connected(false);
        release();
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // LOL
        emit change_dialog("Error occured.", -2, false, false, false);
    }
}

void Authorization_tcp_client::on_response_received(const boost::system::error_code& ec, size_t bytes_transferred)
{
    if(ec.value() == 0) {
        parse_response(bytes_transferred);
        process_data();
    } else {
        set_is_connected(false);
        release();
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // LOL
        emit change_dialog("Error occured.", -2, false, false, false);
    }
}

void Authorization_tcp_client::change_avatar(const QString& img_path)
{
    if(!get_is_connected()) {
        emit create_dialog("Connection error.", -2, false, false, false);
    }
    if(occupy()) {
        if(create_req_for_change_avatar(img_path)) {
            emit create_dialog("Please wait", -2, true, false, false);
            async_write();
        }
        else {
            emit create_dialog("Cant open file", 2000, false, true, true);
            release();
        }
    }
}

bool Authorization_tcp_client::create_req_for_change_avatar(const QString& img_path)
{
    QString processed_img_path = img_path;
    processed_img_path.remove("file://");
    m_avatar_path = processed_img_path;

    QJsonObject j_obj;

    j_obj.insert(Protocol_keys::request, (int)Protocol_codes::Request_code::change_avatar);
    j_obj.insert(Protocol_keys::nickname, m_user_validator.get_nickname());

    QFile img_file(processed_img_path);
    QByteArray base64_img;
    if(img_file.open(QIODevice::ReadOnly)) {
        QByteArray b_arr = img_file.readAll();
        base64_img = b_arr.toBase64();
        img_file.close();
    }
    if(base64_img.isEmpty()) return false;

    j_obj.insert(Protocol_keys::avatar, QString::fromLatin1(base64_img));

    QJsonDocument j_doc(j_obj);

    std::string temp = j_doc.toJson().constData();
    temp += Protocol_keys::end_of_message.toStdString();
    m_session->m_request = temp;

    return true;
}
