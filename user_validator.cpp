#include "user_validator.h"

const QString User_validator::nickname_key = "nickname";
const QString User_validator::password_key = "password";
const QString User_validator::user_info_file_name = "user_info";

User_validator::User_validator(QObject *parent)
    : QObject(parent),
      m_is_authorized(false)
{
    load_user_info();
}

void User_validator::set_nickname(const QString& nickname)
{
    m_nickname = nickname;
}

void User_validator::set_password(const QString& password)
{
    m_password = password;
}

bool User_validator::save_user_info()
{
    QFile file(get_path_to_user_info_file());
    if(file.open(QIODevice::WriteOnly)) {
        QJsonObject j_obj;

        m_is_authorized = true;
        j_obj.insert(nickname_key, m_nickname);
        j_obj.insert(password_key, m_password);

        QJsonDocument j_doc(j_obj);
        file.write(j_doc.toJson());
        file.close();
        return true;
    } else {
        return false;
    }
}

QString User_validator::get_path_to_user_info_file() const
{
    return QCoreApplication::applicationDirPath() + '/' + user_info_file_name;
}

void User_validator::load_user_info()
{
    QFile file(get_path_to_user_info_file());
    if(file.open(QIODevice::ReadOnly)) {
        auto j_doc = QJsonDocument::fromJson(file.readAll());
        file.close();

        if(!j_doc.isEmpty()) {
            auto j_obj = j_doc.object();

            auto j_map = j_obj.toVariantMap();
            m_nickname = j_map[nickname_key].toString();
            m_password = j_map[password_key].toString();
            m_is_authorized = true;
        }
    }
}

bool User_validator::exit_from_account()
{
    QFile file(get_path_to_user_info_file());
    if(file.exists()) {
        if(file.remove()) {
            m_is_authorized = false;
            m_nickname.clear();
            m_password.clear();
            return true;
        }
        else {
            return false;
        }
    }
    return true;
}

