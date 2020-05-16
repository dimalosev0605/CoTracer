#ifndef BASE_TCP_CLIENT_H
#define BASE_TCP_CLIENT_H

#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include "user_validator.h"
#include "Session.h"
#include "Protocol_keys.h"

#include <mutex>
#include <thread>

#include <QThread>

class Base_tcp_client : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool is_authenticated READ get_is_authenticated WRITE set_is_authenticated NOTIFY is_authenticated_changed)
    Q_PROPERTY(bool is_connected READ get_is_connected WRITE set_is_connected NOTIFY is_connected_changed)

    boost::asio::io_service m_ios;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> m_work;
    std::unique_ptr<std::thread> m_thread;

    bool m_is_authenticated = false;

    mutable std::mutex m_is_conencted_mutex;
    bool m_is_connected = false;


    std::mutex m_is_free_mutex;
    bool m_is_free = true;

protected:
    std::unique_ptr<Session> m_session;
    User_validator m_user_validator;

public:
    explicit Base_tcp_client(QObject *parent = nullptr);
    virtual ~Base_tcp_client();

    bool get_is_authenticated() const { return m_is_authenticated; }
    void set_is_authenticated(bool v);

    bool get_is_connected() const;
    void set_is_connected(bool v);

    bool occupy();
    void release();

public slots:
    void cancel_operation();
    void connect_to_server();

signals:
    void is_authenticated_changed();
    void is_connected_changed();

    void info(const QString& info_message, bool is_static);

    // Animation.Infinite = -2
    void create_dialog(const QString& m_message,
                       int m_opacity_anim_duration,
                       bool m_is_busy_indicator_running,
                       bool m_is_opacity_anim_running,
                       bool m_is_destroy);
    void change_dialog(const QString& m_message,
                       int m_opacity_anim_duration,
                       bool m_is_busy_indicator_running,
                       bool m_is_opacity_anim_running,
                       bool m_is_destroy);
};

#endif // BASE_TCP_CLIENT_H
