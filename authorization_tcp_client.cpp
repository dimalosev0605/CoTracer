#include "authorization_tcp_client.h"

Authorization_tcp_client::Authorization_tcp_client(QObject* parent)
    : Base_tcp_client(parent)
{

}

void Authorization_tcp_client::sing_in(const QString& nickname, const QString& password)
{
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
}

void Authorization_tcp_client::sing_up(const QString& nickname, const QString& password)
{
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

void Authorization_tcp_client::parse_response()
{
    auto j_doc = QJsonDocument::fromJson((char*)m_session->m_response.data().data());
    m_session->m_response.consume(UINT_MAX);

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
        emit success_sign_up();
        break;
    }
    case Protocol_codes::Response_code::sign_up_failure: {
        emit sign_up_failure();
        break;
    }
    case Protocol_codes::Response_code::success_sign_in: {
        m_user_validator.save_user_info();
        set_is_authenticated(true);
        emit success_sign_in();
        break;
    }
    case Protocol_codes::Response_code::sign_in_failure: {
        emit sign_in_failure();
        break;
    }
    case Protocol_codes::Response_code::internal_server_error: {
        emit internal_server_error();
        break;
    }

    }
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
    }
}

void Authorization_tcp_client::on_response_received(const boost::system::error_code& ec, size_t bytes_transferred)
{
    if(ec.value() == 0) {
        parse_response();
        process_data();
    } else {

    }
}
