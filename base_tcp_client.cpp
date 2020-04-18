#include "base_tcp_client.h"

Base_tcp_client::Base_tcp_client(QObject *parent)
    : QObject(parent),
      m_work(boost::asio::make_work_guard(m_ios))
{
    m_thread.reset(new std::thread([this]()
    {
//        qDebug() << "network thread" << QThread::currentThreadId();
        m_ios.run();
    }));

//    qDebug() << "Base ctor";
//    qDebug() << "main thread: " << QThread::currentThreadId();

    connect_to_server();
    check_authentication();
}

Base_tcp_client::~Base_tcp_client()
{
    m_thread->detach();
    m_work.reset();
//    qDebug() << "Base dtor";
}

void Base_tcp_client::connect_to_server()
{
    m_session = std::unique_ptr<Session>(new Session(m_ios, "127.0.0.1", 1234));
    m_session->m_socket.open(m_session->m_ep.protocol());
    m_session->m_socket.async_connect(m_session->m_ep, [this](const boost::system::error_code& ec)
    {
//        qDebug() << "callback_thread: " << QThread::currentThreadId();
        if(ec.value() == 0) {
            set_is_connected(true);
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
    m_session->m_response.consume(UINT_MAX); // ЫЫЫ
    release();
}

bool Base_tcp_client::is_free() const
{
    std::lock_guard<std::mutex> lock(m_is_free_mutex);
    return m_is_free;
}

void Base_tcp_client::occupy()
{
    std::lock_guard<std::mutex> lock(m_is_free_mutex);
    m_is_free = false;
}

void Base_tcp_client::release()
{
    std::lock_guard<std::mutex> lock(m_is_free_mutex);
    m_is_free = true;
}
