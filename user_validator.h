#ifndef USER_VALIDATOR_H
#define USER_VALIDATOR_H

#include <QObject>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QCoreApplication>
#include <QDataStream>
#include <QJsonObject>
#include <QJsonDocument>

class User_validator : public QObject
{
    Q_OBJECT

    static const QString nickname_key;
    static const QString password_key;
    static const QString user_info_file_name;
    static const QString user_avatar;
    static const QString user_files;

private:
    QString m_nickname;
    QString m_password;
    QString m_avatar_path;
    bool m_is_authorized;

private:
    void create_user_files_dir();
    void load_user_info();
    QString get_path_to_user_files_dir() const;

public:
    explicit User_validator(QObject *parent = nullptr);

    bool get_is_authorized() const { return m_is_authorized; }

    QString get_nickname() const { return m_nickname; }
    QString get_password() const { return m_password; }
    QString get_avatar_path() const { return m_avatar_path; }
    QString get_avatar_path_for_saving() const;
    void save_avatar_path(const QString& path);

    void set_nickname(const QString& nickname);
    void set_password(const QString& password);

    bool save_user_info();
    bool exit_from_account();
    bool save_avatar(const QString& img_path);

signals:
};

#endif // USER_VALIDATOR_H
