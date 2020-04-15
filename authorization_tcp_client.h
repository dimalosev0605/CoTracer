#ifndef AUTHORIZATION_TCP_CLIENT_H
#define AUTHORIZATION_TCP_CLIENT_H

#include "base_tcp_client.h"

#include "boost/bind/bind.hpp"

class Authorization_tcp_client: public Base_tcp_client
{
    Q_OBJECT

private:
    const char* create_request(Protocol_codes::Request_code code, const QString& nickname, const QString& password);

    void parse_response();
    void process_data();

    void on_request_sent(const boost::system::error_code& ec, size_t bytes_transferred);
    void on_response_received(const boost::system::error_code& ec, size_t bytes_transferred);

public:
    explicit Authorization_tcp_client(QObject* parent = nullptr);

public slots:
    void sing_in(const QString& nickname, const QString& password);
    void sing_up(const QString& nickname, const QString& password);
    void exit_from_account();

    QString get_nickname() const { return m_user_validator.get_nickname(); }
    QString get_password() const { return m_user_validator.get_password(); }

signals:
    void success_sign_up();
    void sign_up_failure();
    void success_sign_in();
    void sign_in_failure();
    void internal_server_error();
};

#endif // AUTHORIZATION_TCP_CLIENT_H
