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

#include "fad_manager.h"

class User_validator
{
    static const QString nickname_key;
    static const QString password_key;

private:
    bool m_is_authorized;

    QString m_nickname;
    QString m_password;
    QString m_new_password;
    QString m_new_avatar_path;

    FAD_manager m_fad_manager;
    Path_finder m_path_finder;

private:
    void load_user_info();

public:
    User_validator();

    bool get_is_authorized() const { return m_is_authorized; }

    QString get_nickname() const { return m_nickname; }
    QString get_password() const { return m_password; };
    QString get_new_password() const { return m_new_password; }
    QString get_new_avatar_path() const { return m_new_avatar_path; }

    void set_nickname(const QString& nickname);
    void set_password(const QString& password);
    void set_new_password(const QString& new_password);
    void set_new_avatar_path(const QString& path);

    bool save_user_info();
    bool save_user_avatar(const QByteArray& avatar);
    bool exit_from_account();
    bool reset_user_avatar();
};

#endif // USER_VALIDATOR_H
