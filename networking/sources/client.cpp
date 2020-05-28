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
    if(!get_is_connected()) {
        connect_to_server();
        return false;
    }
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

bool Client::get_is_connected()
{
    std::lock_guard<std::mutex> lock(m_is_connected_mutex);
    return m_is_connected;
}

void Client::set_is_connected(bool value)
{
    std::lock_guard<std::mutex> lock(m_is_connected_mutex);
    m_is_connected = value;
    emit is_connected_changed();
}

bool Client::get_is_authorized()
{
    std::lock_guard<std::mutex> lock(m_is_authorized_mutex);
    return m_is_authorized;
}

void Client::set_is_authorized(bool value)
{
    std::lock_guard<std::mutex> lock(m_is_authorized_mutex);
    m_is_authorized = value;
    emit is_authorized_changed();
}
