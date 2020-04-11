#ifndef AUTHENTICATION_TCP_CLIENT_H
#define AUTHENTICATION_TCP_CLIENT_H

#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include <boost/bind/bind.hpp>

#include "user_validator.h"
#include "authentication_session.h"

class Authentication_tcp_client : public QObject
{
    Q_OBJECT
    Q_PROPERTY(bool is_authenticated READ get_is_authenticated WRITE set_is_authenticated NOTIFY is_authenticated_changed)

    boost::asio::io_service m_ios;
    std::unique_ptr<boost::asio::io_service::work> m_work;
    std::unique_ptr<std::thread> m_thread;
    std::unique_ptr<Authentication_session> m_session;
    User_validator m_user_validator;
    bool m_is_authenticated = false;

private:
    void connect_to_server();
    void check_authentication();
    const char* create_request(const QString& nickname, const QString& password);

    void async_write_callback(const boost::system::error_code& ec, size_t bytes_transferred);
    void async_read_until_callback(const boost::system::error_code &ec, size_t bytes_transferred);

    void parse_response();
    void process_data();

public:
    explicit Authentication_tcp_client(QObject *parent = nullptr);
    ~Authentication_tcp_client();

    bool get_is_authenticated() const { return m_is_authenticated; }
    void set_is_authenticated(bool v);

public slots:
    void sing_in(const QString& nickname, const QString& password);
    void sing_up(const QString& nickname, const QString& password);
    void cancel_operation();
    void exit_from_account();

    QString get_nickname() const { return m_user_validator.get_nickname(); }
    QString get_password() const { return m_user_validator.get_password(); }

signals:
    void is_authenticated_changed();

    void success_connection();
    void connection_error();
    void success_sign_up();
    void sign_up_failure();
    void success_sign_in();
    void sign_in_failure();
    void internal_server_error();
};

#endif // AUTHENTICATION_TCP_CLIENT_H
