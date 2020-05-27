#ifndef AUTHORIZATION_TCP_CLIENT_H
#define AUTHORIZATION_TCP_CLIENT_H

#include "base_tcp_client.h"
#include "miscellaneous/headers/path_finder.h"

#include "boost/bind/bind.hpp"

class Authorization_tcp_client: public Base_tcp_client
{
    Q_OBJECT

    QString m_avatar_path;
    QByteArray m_avatar;
    Path_finder m_path_finder;
    QString m_new_password;

private:
    const char* create_request(Protocol_codes::Request_code code, const QString& nickname, const QString& password);

    void parse_response(std::size_t bytes_transferred);
    void process_data();

    void on_request_sent(const boost::system::error_code& ec, size_t bytes_transferred);
    void on_response_received(const boost::system::error_code& ec, size_t bytes_transferred);

    void async_write(); // mb add in base class?
    void parse_avatar(QMap<QString, QVariant>& j_map);

public:
    explicit Authorization_tcp_client(QObject* parent = nullptr);
    ~Authorization_tcp_client();

public slots:
    void sign_in(const QString& nickname, const QString& password);
    void sign_up(const QString& nickname, const QString& password);
    bool exit_from_account();

    void change_avatar(const QString& img_path);
    bool create_req_for_change_avatar(const QString& img_path);

    void change_password(const QString& new_password);

    QString get_nickname() const { return m_user_validator.get_nickname(); }
    QString get_password() const { return m_user_validator.get_password(); }
    QString get_avatar_path(bool prefix) const { return m_path_finder.get_path_to_user_avatar(prefix); }

signals:
    void update_password_field();
};

#endif // AUTHORIZATION_TCP_CLIENT_H
