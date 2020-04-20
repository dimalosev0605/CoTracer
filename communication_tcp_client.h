#ifndef COMMUNICATION_TCP_CLIENT_H
#define COMMUNICATION_TCP_CLIENT_H


#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>

#include <boost/bind/bind.hpp>

#include "user_validator.h"
#include "base_tcp_client.h"
#include "contacts_model.h"

class Communication_tcp_client: public Base_tcp_client
{
    Q_OBJECT

    QList<Contacts_model*> m_models;

    QString m_model_date;
    QString m_model_nickname;

    int m_index_for_deletion;

    User_validator m_user_validator;
    QVector<std::pair<QString, QString>> m_received_contacts;

private:
    const char* create_add_contact_req(Protocol_codes::Request_code code, const QString& nickname, const QString& time);

    void on_request_sent(const boost::system::error_code& ec, size_t bytes_transferred);
    void on_response_received(const boost::system::error_code& ec, size_t bytes_transferred);

    void request_contacts(Protocol_codes::Request_code code);
    const char* create_req_for_contacts(Protocol_codes::Request_code code);

    const char* create_remove_contact_req(Protocol_codes::Request_code code, const QString& nickname, const QString& time);

    void parse_response(size_t bytes_transferred);
    void parse_contacts(Protocol_codes::Response_code code, const QMap<QString, QVariant>& j_map);

    void process_data();

public:
    explicit Communication_tcp_client(QObject* parent = nullptr);

public slots:
    Contacts_model* create_model_based_on_date(const QString& date);
    Contacts_model* create_model_based_on_nickname(const QString& nickname);
    void destroy_model();

    void add_contact(int code, const QString& nickname, const QString& time);
    bool remove_contact(int code, const QString& nickname, const QString& time, int index);

signals:
    void success_adding();
    void success_register_contact_deletion(int index);
    void success_unregister_contact_deletion(int index);
    void register_contact_deletion_failure();
    void unregister_contact_deletion_failure();
    void unregistered_list(const QVector<std::pair<QString, QString>>&);
    void registered_list(const QVector<std::pair<QString, QString>>&);
};

#endif // COMMUNICATION_TCP_CLIENT_H
