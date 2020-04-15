#include "base_tcp_client.h"

Base_tcp_client::Base_tcp_client(QObject *parent)
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

Base_tcp_client::~Base_tcp_client()
{
    m_work.reset(nullptr);
    m_thread->detach();
}

void Base_tcp_client::connect_to_server()
{
    m_session = std::unique_ptr<Session>(new Session(m_ios, "127.0.0.1", 1234));
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

void Base_tcp_client::check_authentication()
{
    if(m_user_validator.get_is_authorized()) {
        set_is_authenticated(true);
    }
    else {
        set_is_authenticated(false);
    }
}

void Base_tcp_client::set_is_authenticated(bool v)
{
    m_is_authenticated = v;
    emit is_authenticated_changed();
}

void Base_tcp_client::cancel_operation()
{
    m_session->m_socket.cancel();
    m_session->m_request.clear();
    m_session->m_response.consume(UINT_MAX);
}
