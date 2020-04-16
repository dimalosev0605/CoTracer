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

    QList<Contacts_model*> m_contacts_models;

    QString m_date;
    QString m_nickname;

    User_validator m_user_validator;
    QVector<std::pair<QString, QString>> m_unregistered_contacts;
    QVector<std::pair<QString, QString>> m_registered_contacts;

private:
    const char* add_reg_user_req(const QString& nickname, const QString& time);
    const char* add_unreg_user_req(const QString& nickname, const QString& time);

    void on_request_sent(const boost::system::error_code& ec, size_t bytes_transferred);
    void on_response_received(const boost::system::error_code& ec, size_t bytes_transferred);

    void get_all_contacts_with_registered();
    void get_all_contacts_with_unregistered();

    const char* get_all_contacts_with_registered_req();
    const char* get_all_contacts_with_unregistered_req();

    const char* remove_unregistered_contact_req(const QString& nickname, const QString& time);
    const char* remove_registered_contact_req(const QString& nickname, const QString& time);

    void parse_response(size_t bytes_transferred);
    void process_data();

public:
    explicit Communication_tcp_client(QObject* parent = nullptr);

public slots:
    Contacts_model* create_main_model(const QString& date);
    void destroy_model();

    void add_registered_contact(const QString& nickaname, const QString& time);
    void add_unregistered_contact(const QString& nickaname, const QString& time);

    void remove_registered_contact(const QString& nickname, const QString& time);
    void remove_unregistered_contact(const QString& nickname, const QString& time);

    Contacts_model* create_new_view(const QString& nickname);

signals:
    void success_adding();
    void internal_server_error();
    void unregistered_list(const QVector<std::pair<QString, QString>>&);
    void registered_list(const QVector<std::pair<QString, QString>>&);
};

#endif // COMMUNICATION_TCP_CLIENT_H
