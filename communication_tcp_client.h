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
    QVector<QString> m_previous_links;
    QVector<int> m_previous_links_count;

    QString m_add_contact_nickname;
    QString m_add_contact_time;
    bool m_add_contact_is_reg;

    int m_index_for_deletion;

    User_validator m_user_validator;
    QVector<std::tuple<QString, QString, bool>> m_received_contacts;
    QVector<std::tuple<QString, int, int>> m_stats; // date, reg, unreg.

private:
    const char* create_add_contact_req(Protocol_codes::Request_code code, const QString& nickname, const QString& time);

    void on_request_sent(const boost::system::error_code& ec, size_t bytes_transferred);
    void on_response_received(const boost::system::error_code& ec, size_t bytes_transferred);

    void request_contacts();
    const char* create_request_for_contacts();

    const char* create_remove_contact_req(Protocol_codes::Request_code code, const QString& nickname, const QString& time);

    void parse_response(size_t bytes_transferred);
    void parse_contacts(const QMap<QString, QVariant>& j_map);

    void process_data();

    const char* create_req_for_14_days_stats();
    void parse_stat(const QMap<QString, QVariant>& j_map);

    void async_write();

private slots:

public:
    explicit Communication_tcp_client(QObject* parent = nullptr);

public slots:
    Contacts_model* create_model_based_on_date(const QString& date);
    Contacts_model* create_model_based_on_nickname(const QString& nickname);

    void destroy_model();

    bool add_contact(int code, const QString& nickname, const QString& time);
    bool remove_contact(int code, const QString& nickname, const QString& time, int index);

    void request_for_14_days_stats();

signals:
    void success_adding(const QString& nickname, const QString& time, bool is_reg);
    void success_contact_deletion(int index);
    void contacts_received(const QVector<std::tuple<QString, QString, bool>>& contacts);
    void fetching_statistics(const QString& message);
    void statistics_received(const QVector<std::tuple<QString, int, int>>& m_stats);
};

#endif // COMMUNICATION_TCP_CLIENT_H
