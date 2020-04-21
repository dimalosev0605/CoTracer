#include "authorization_tcp_client.h"

Authorization_tcp_client::Authorization_tcp_client(QObject* parent)
    : Base_tcp_client(parent)
{
//    qDebug() << "Derived ctor";
}

Authorization_tcp_client::~Authorization_tcp_client()
{
//    qDebug() << "Derived dtor";
}

bool Authorization_tcp_client::sing_in(const QString& nickname, const QString& password)
{
    if(!get_is_connected()) {
        emit info("Connection error");
        return false;
    }
    if(occupy()) {
        m_user_validator.set_nickname(nickname);
        m_user_validator.set_password(password);

        m_session->m_request = create_request(Protocol_codes::Request_code::sign_in, nickname, password);

        boost::asio::async_write(m_session->m_socket, boost::asio::buffer(m_session->m_request),
                                 boost::bind
                                 (&Authorization_tcp_client::on_request_sent,
                                  boost::ref(*this),
                                  boost::placeholders::_1,
                                  boost::placeholders::_2)
                                 );
        return true;
    } else {
        return false;
    }
}

bool Authorization_tcp_client::sing_up(const QString& nickname, const QString& password)
{
    if(!get_is_connected()) {
        emit info("Connection error");
        return false;
    }
    if(occupy()) {
        m_user_validator.set_nickname(nickname);
        m_user_validator.set_password(password);

        m_session->m_request = create_request(Protocol_codes::Request_code::sign_up, nickname, password);

        boost::asio::async_write(m_session->m_socket, boost::asio::buffer(m_session->m_request),
                                 boost::bind
                                 (&Authorization_tcp_client::on_request_sent,
                                  boost::ref(*this),
                                  boost::placeholders::_1,
                                  boost::placeholders::_2)
                                 );
        return true;
    } else {
        return false;
    }
}

void Authorization_tcp_client::exit_from_account()
{
    if(m_user_validator.exit_from_account()) {
        set_is_authenticated(false);
    }
    else {
        set_is_authenticated(true);
    }
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
    }
}

void Authorization_tcp_client::process_data()
{
    switch (m_session->m_res_code) {

    case Protocol_codes::Response_code::success_sign_up: {
        m_user_validator.save_user_info();
        set_is_authenticated(true);
        emit info("Success sing up!");
        break;
    }
    case Protocol_codes::Response_code::sign_up_failure: {
        emit info("Such user already exists.");
        break;
    }
    case Protocol_codes::Response_code::success_sign_in: {
        m_user_validator.save_user_info();
        set_is_authenticated(true);
        emit info("Success sign in!");
        break;
    }
    case Protocol_codes::Response_code::sign_in_failure: {
        emit info("Incorrect nickname or password.");
        break;
    }
    case Protocol_codes::Response_code::internal_server_error: {
        emit info("Internal server error occured. Try later.");
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
        emit info("Error occured.");
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
        emit info("Error occured.");
    }
}
