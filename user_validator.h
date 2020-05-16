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

#include "path_finder.h"

class User_validator : public QObject
{
    Q_OBJECT

    static const QString nickname_key;
    static const QString password_key;

private:
    bool m_is_authorized;

    QString m_nickname;
    QString m_password;

    Path_finder m_path_finder;

private:
    void create_user_files_dir();
    void load_user_info();

public:
    explicit User_validator(QObject *parent = nullptr);

    bool get_is_authorized() const { return m_is_authorized; }

    QString get_nickname() const { return m_nickname; }
    QString get_password() const { return m_password; };

    void set_nickname(const QString& nickname);
    void set_password(const QString& password);

    bool save_user_info();
    bool save_user_avatar(const QByteArray& avatar);
    bool exit_from_account();
    bool save_avatar(const QString& img_path);
};

#endif // USER_VALIDATOR_H
