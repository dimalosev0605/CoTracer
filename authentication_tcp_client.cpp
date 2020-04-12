#include "authentication_tcp_client.h"

Authentication_tcp_client::Authentication_tcp_client(QObject *parent)
    : QObject(parent)
{
    m_work.reset(new boost::asio::io_service::work(m_ios));
    m_thread.reset(new std::thread([this]()
    {
        m_ios.run();
    }));

    connect_to_server();
    check_authentication();
}

Authentication_tcp_client::~Authentication_tcp_client()
{
    m_work.reset(nullptr);
    m_thread->detach();
}

void Authentication_tcp_client::connect_to_server()
{
    m_session = std::unique_ptr<Authentication_session>(new Authentication_session(m_ios, "127.0.0.1", 1234));
    m_session->m_socket.open(m_session->m_ep.protocol());
    m_session->m_socket.async_connect(m_session->m_ep, [this](const boost::system::error_code& ec)
    {
        if(ec.value() == 0) {
            emit success_connection();
        } else {
            emit connection_error();
        }
    });
}

void Authentication_tcp_client::check_authentication()
{
    if(m_user_validator.get_is_authorized()) {
        set_is_authenticated(true);
    }
}

void Authentication_tcp_client::set_is_authenticated(bool v)
{
    m_is_authenticated = v;
    emit is_authenticated_changed();
}

void Authentication_tcp_client::sing_in(const QString& nickname, const QString& password)
{
    m_user_validator.set_nickname(nickname);
    m_user_validator.set_password(password);

    m_session->m_req_code = Protocol_codes::Request_code::sign_in;
    m_session->m_request = create_request(nickname, password);

    boost::asio::async_write(m_session->m_socket, boost::asio::buffer(m_session->m_request),
                             boost::bind
                             (&Authentication_tcp_client::async_write_callback,
                              boost::ref(*this),
                              boost::placeholders::_1,
                              boost::placeholders::_2)
                             );
}

void Authentication_tcp_client::sing_up(const QString& nickname, const QString& password)
{
    m_user_validator.set_nickname(nickname);
    m_user_validator.set_password(password);

    m_session->m_req_code = Protocol_codes::Request_code::sign_up;
    m_session->m_request = create_request(nickname, password);

    boost::asio::async_write(m_session->m_socket, boost::asio::buffer(m_session->m_request),
                             boost::bind
                             (&Authentication_tcp_client::async_write_callback,
                              boost::ref(*this),
                              boost::placeholders::_1,
                              boost::placeholders::_2)
                             );
}

void Authentication_tcp_client::cancel_operation()
{
    m_session->m_socket.cancel();
    m_session->m_request.clear();
    m_session->m_response.consume(UINT_MAX);
}

void Authentication_tcp_client::exit_from_account()
{
    cancel_operation();
    if(m_user_validator.exit_from_account()) {
        set_is_authenticated(false);
    }
}

const char* Authentication_tcp_client::create_request(const QString& nickname, const QString& password)
{
    QJsonObject j_obj;

    j_obj.insert("request", (int)m_session->m_req_code);
    j_obj.insert("nickname", nickname);
    j_obj.insert("password", password);

    QJsonDocument j_doc(j_obj);
    return j_doc.toJson().append("\r\n\r\n").data();
}

void Authentication_tcp_client::async_write_callback(const boost::system::error_code& ec, size_t bytes_transferred)
{
    m_session->m_request.clear();
    if(ec.value() == 0) {
        boost::asio::async_read_until(m_session->m_socket, m_session->m_response, "\r\n\r\n",
                                boost::bind
                                (&Authentication_tcp_client::async_read_until_callback,
                                 boost::ref(*this),
                                 boost::placeholders::_1,
                                 boost::placeholders::_2)
                                );
    } else {
        if(ec == boost::asio::error::operation_aborted) {
        } else {
        }
    }
}

void Authentication_tcp_client::async_read_until_callback(const boost::system::error_code &ec, size_t bytes_transferred)
{
    if(ec.value() == 0) {
        parse_response();
        process_data();
    } else {
        if(ec == boost::asio::error::operation_aborted) {
        } else {
        }
    }
}

void Authentication_tcp_client::parse_response()
{
    auto j_doc = QJsonDocument::fromJson((char*)m_session->m_response.data().data());
    m_session->m_response.consume(UINT_MAX);

    if(!j_doc.isEmpty()) {
        auto j_obj = j_doc.object();
        auto j_map = j_obj.toVariantMap();
        m_session->m_res_code = (Protocol_codes::Response_code)j_map["response"].toInt();
    }
}

void Authentication_tcp_client::process_data()
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
