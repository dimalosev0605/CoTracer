#include "base_tcp_client.h"

Base_tcp_client::Base_tcp_client(QObject *parent)
    : QObject(parent),
      m_work(boost::asio::make_work_guard(m_ios))
{
    m_thread.reset(new std::thread([this]()
    {
        m_ios.run();
    }));
    set_is_authenticated(m_user_validator.get_is_authorized());
}

Base_tcp_client::~Base_tcp_client()
{
    m_thread->detach();
    m_work.reset();
}

void Base_tcp_client::connect_to_server()
{
    emit create_dialog("Connecting to server...", -2, true, false, false);
    m_session = std::unique_ptr<Session>(new Session(m_ios, "192.168.1.140", 1234));
    m_session->m_socket.open(m_session->m_ep.protocol());
    m_session->m_socket.async_connect(m_session->m_ep, [this](const boost::system::error_code& ec)
    {
        if(ec.value() == 0) {
            set_is_connected(true);
            emit change_dialog("Success connection to server!", 2000, false, true, true);
        } else {
            std::this_thread::sleep_for(std::chrono::milliseconds(500)); // LOL
            emit change_dialog("Connection error.", -2, false, false, false);
        }
    });
}

void Base_tcp_client::set_is_authenticated(bool v)
{
    m_is_authenticated = v;
    emit is_authenticated_changed();
}

bool Base_tcp_client::get_is_connected() const
{
    std::lock_guard<std::mutex> lock(m_is_conencted_mutex);
    return m_is_connected;
}

void Base_tcp_client::set_is_connected(bool v)
{
    std::lock_guard<std::mutex> lock(m_is_conencted_mutex);
    m_is_connected = v;
    emit is_connected_changed();
}

void Base_tcp_client::cancel_operation()
{
    m_session->m_socket.cancel();
    m_session->m_request.clear();
    m_session->m_response.consume(UINT_MAX); // LOL
    release();
}

bool Base_tcp_client::occupy()
{
    std::lock_guard<std::mutex> lock(m_is_free_mutex);
    if(m_is_free) {
        m_is_free = false;
        return true;
    }
    else {
        return false;
    }
}

void Base_tcp_client::release()
{
    std::lock_guard<std::mutex> lock(m_is_free_mutex);
    m_is_free = true;
}
