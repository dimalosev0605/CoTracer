#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>

#include <mutex>
#include <thread>
#include <tuple>
#include <any>

#include "boost/bind/bind.hpp"
#include "boost/asio.hpp"

#include "./miscellaneous/headers/user_validator.h"
#include "./miscellaneous/headers/path_finder.h"
#include "./models/headers/contacts_model.h"
#include "Session.h"
#include "Protocol_codes.h"
#include "Protocol_keys.h"

class Client: public QObject
{
    Q_OBJECT

    Q_PROPERTY(bool is_authorized READ get_is_authorized WRITE set_is_authorized NOTIFY is_authorized_changed)
    bool m_is_authorized = false;

    boost::asio::io_service m_ios;
    boost::asio::executor_work_guard<boost::asio::io_context::executor_type> m_work;
    std::unique_ptr<Session> m_session;
    std::unique_ptr<std::thread> m_thread;

    bool m_is_sock_free = true;
    std::mutex m_is_sock_free_mutex;

    bool m_is_connected = false;
    std::mutex m_is_connected_mutex;

    User_validator m_user_validator;
    Path_finder m_path_finder;

    QVector<Contacts_model*> m_models;
    std::vector<std::any> lol_vector;

    std::vector<std::tuple<QString, QString>> m_cached_avatars;

private:
    void connect_to_server();
    bool occupy_sock();
    void free_sock();
    void async_write();
    void on_request_sent(const boost::system::error_code& ec, size_t bytes_transferred);
    void on_response_received(const boost::system::error_code& ec, size_t bytes_transferred);

    // create requests functions
    void create_sign_in_req(const QString& nickname, const QString& password);
    void create_sign_up_req(const QString& nickname, const QString& password);
    void create_change_password_req(const QString& new_password);
    bool create_change_avatar_req(const QString& new_avatar_path);
    void create_set_default_avatar_req();
    void create_fetch_stat_for_14_days_req();
    void create_fetch_contacts_req(const QString& nickname, const QString& date);
    void create_add_contact_req(const QString& nickname, const QString& time, const QString& date);
    void create_remove_contact_req(const QString& nickname, const QString& time, const QString& date);

    // process response functions
    void process_data(std::size_t bytes_transferred);
    void process_success_sign_in(QMap<QString, QVariant>& j_map);
    void process_success_sign_up();
    void process_success_password_changing();
    void process_success_avatar_changing();
    void process_success_setting_default_avatar();
    void process_success_fetching_stat_for_14_days(QMap<QString, QVariant>& j_map);
    void process_success_fetching_contacts(QMap<QString, QVariant>& j_map);
    void process_success_contact_adding(QMap<QString, QVariant>& j_map);
    void process_such_contact_not_exists();
    void process_success_contact_deletion();
    void process_internal_server_error();

    // miscellaneous
    void fetch_contacts(const QString& nickname, const QString& date);
    void insert_info_about_cached_avatars_in_req(QJsonObject& j_obj);

    void read_cached_avatars_info_file();
    void save_cached_avatars_info_file();
    void delete_old_cached_avatars();

public:
    explicit Client(QObject* parent = nullptr);
    ~Client();

    bool get_is_connected();
    void set_is_connected(bool value);

    bool get_is_authorized();
    void set_is_authorized(bool value);

public slots:
    void sign_in(const QString& nickname, const QString& password);
    void sign_up(const QString& nickname, const QString& password);
    void change_password(const QString& new_password);
    void change_avatar(const QString& new_avatar_path);
    void set_default_avatar();
    bool exit_from_account();

    void fetch_stat_for_14_days();
    void add_contact(const QString& nickname, const QString& time, const QString& date);
    void remove_contact(const QString& nickname, const QString& time, const QString& date, int index_in_table);
    Contacts_model* create_model_based_on_date(const QString& date);
    Contacts_model* create_model_based_on_date_and_nickname(const QString& nickname, const QString& date);
    void pop_model();
    void cancel_operation();

    QString get_nickname() const;
    QString get_password() const;
    QString get_avatar_path(bool prefix) const;

signals:
    void is_connected_changed();
    void is_authorized_changed();
    void update_password_field();
    void success_setting_default_avatar();

    void statistic_received(const QVector<std::tuple<QString, int>>& statistic);
    void contacts_received(const QVector<std::tuple<QString, QString>>& contacts);
    void success_contact_adding(const QString& nickname, const QString& time);
    void success_contact_deletion(int index);
};

#endif // CLIENT_H
