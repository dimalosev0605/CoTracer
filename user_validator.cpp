#include "user_validator.h"

const QString User_validator::nickname_key = "nickname";
const QString User_validator::password_key = "password";

User_validator::User_validator(QObject *parent)
    : QObject(parent),
      m_is_authorized(false)
{
    create_user_files_dir();
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
    create_user_files_dir();
    QFile file(m_path_finder.get_path_to_user_info_file());
    if(file.open(QIODevice::WriteOnly)) {
        QJsonObject j_obj;

        m_is_authorized = true;
        j_obj.insert(nickname_key, m_nickname);
        j_obj.insert(password_key, m_password);

        QJsonDocument j_doc(j_obj);
        file.write(j_doc.toJson());

        return true;
    } else {
        return false;
    }
}

bool User_validator::save_user_avatar(const QByteArray& avatar)
{
    create_user_files_dir();
    QFile file(m_path_finder.get_path_to_user_avatar(false));
    if(file.open(QIODevice::WriteOnly)) {
        file.write(avatar);
        return true;
    }
    return false;
}

void User_validator::load_user_info()
{
    QFile file(m_path_finder.get_path_to_user_info_file());
    if(file.open(QIODevice::ReadOnly)) {
        auto j_doc = QJsonDocument::fromJson(file.readAll());

        if(!j_doc.isEmpty()) {
            auto j_obj = j_doc.object();

            auto j_map = j_obj.toVariantMap();
            m_nickname = j_map[nickname_key].toString();
            m_password = j_map[password_key].toString();
            if(!m_nickname.isEmpty() && !m_password.isEmpty()) {
                m_is_authorized = true;
            }
            else {
                m_nickname.clear();
                m_password.clear();
            }
        }
    }
}

bool User_validator::exit_from_account()
{
    QDir dir(m_path_finder.get_path_to_user_files_dir());
    if(dir.removeRecursively()) {
        m_is_authorized = false;
        m_nickname.clear();
        m_password.clear();
        return true;
    }
    else {
        return false;
    }
}

bool User_validator::save_avatar(const QString& img_path)
{
    // delete old avatar
    QFile old_avatar(m_path_finder.get_path_to_user_avatar(false));
    old_avatar.remove();
    //

    QFile copy_from_file(img_path);

    return copy_from_file.copy(m_path_finder.get_path_to_user_avatar(false));
}

void User_validator::create_user_files_dir()
{
    QDir dir(m_path_finder.get_app_dir_path());
    dir.mkdir(m_path_finder.get_path_to_user_files_dir());
    dir.mkdir(m_path_finder.get_path_to_avatars_dir());
}
