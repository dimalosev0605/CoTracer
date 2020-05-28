#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QThread>

#include <mutex>
#include <thread>

#include "./miscellaneous/headers/user_validator.h"
#include "Session.h"
#include "Protocol_keys.h"

class Client: public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool is_connected READ get_is_connected WRITE set_is_connected NOTIFY is_connected_changed)
    bool m_is_connected = false;
    std::mutex m_is_connected_mutex;

    Q_PROPERTY(bool is_authorized READ get_is_authorized WRITE set_is_authorized NOTIFY is_authorized_changed)
    bool m_is_authorized = false;
    std::mutex m_is_authorized_mutex;

    boost::asio::io_service m_ios;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> m_work;
    std::unique_ptr<Session> m_session;
    std::unique_ptr<std::thread> m_thread;

    bool m_is_sock_free = true;
    std::mutex m_is_free_sock_mutex;

    User_validator m_user_validator;

private:
    void connect_to_server();
    bool occupy_sock();
    void free_sock();

public:
    explicit Client(QObject* parent = nullptr);

    bool get_is_connected();
    void set_is_connected(bool value);

    bool get_is_authorized();
    void set_is_authorized(bool value);

public slots:

signals:
    void is_connected_changed();
    void is_authorized_changed();
};

#endif // CLIENT_H
