#ifndef BASE_TCP_CLIENT_H
#define BASE_TCP_CLIENT_H

#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include "user_validator.h"
#include "Session.h"
#include "Protocol_keys.h"

class Base_tcp_client : public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool is_authenticated READ get_is_authenticated WRITE set_is_authenticated NOTIFY is_authenticated_changed)

    boost::asio::io_service m_ios;
    std::unique_ptr<boost::asio::io_service::work> m_work;
    std::unique_ptr<std::thread> m_thread;
    bool m_is_authenticated = false;

protected:
    std::unique_ptr<Session> m_session;
    User_validator m_user_validator;

private:
    void connect_to_server();
    void check_authentication();

public:
    explicit Base_tcp_client(QObject *parent = nullptr);
    ~Base_tcp_client(); // maybe virtual?

    bool get_is_authenticated() const { return m_is_authenticated; }
    void set_is_authenticated(bool v);

public slots:
    void cancel_operation();

signals:
    void is_authenticated_changed();
    void success_connection();
    void connection_error();
};

#endif // BASE_TCP_CLIENT_H
