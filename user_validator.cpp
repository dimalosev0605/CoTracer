#include "user_validator.h"

const QString User_validator::nickname_key = "nickname";
const QString User_validator::password_key = "password";
const QString User_validator::user_info_file_name = "user_info";
const QString User_validator::user_avatar = "avatar";
const QString User_validator::user_files = "user_files";

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
    QFile file(get_path_to_user_files_dir() + '/' + user_info_file_name);
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

QString User_validator::get_path_to_user_files_dir() const
{
    return QCoreApplication::applicationDirPath() + '/' + user_files;
}

void User_validator::load_user_info()
{
    m_avatar_path = "file://" + get_path_to_user_files_dir() + '/' + user_avatar;

    QFile file(get_path_to_user_files_dir() + '/' + user_info_file_name);
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
    QDir dir(get_path_to_user_files_dir());
    if(dir.removeRecursively()) {
        m_is_authorized = false;
        m_nickname.clear();
        m_password.clear();
        m_avatar_path.clear();
        return true;
    }
    else {
        return false;
    }
}

bool User_validator::save_avatar(const QString& img_path)
{
    QFile file_for_copy(img_path);
    QString copied_file_name = get_path_to_user_files_dir() + '/' + user_avatar;

    QString path_for_delete = m_avatar_path;
    path_for_delete.remove("file://");
    QFile delete_old_avatar(path_for_delete);
    delete_old_avatar.remove();

    if(file_for_copy.copy(copied_file_name)) {
        m_avatar_path = "file://" + copied_file_name;
        return true;
    }
    return false;
}

void User_validator::create_user_files_dir()
{
    QDir dir(QCoreApplication::applicationDirPath());
    if(!dir.exists(user_files)) {
        dir.mkdir(user_files);
    }
}

QString User_validator::get_avatar_path_for_saving() const
{
    return QCoreApplication::applicationDirPath() + '/' + user_files + '/' + user_avatar;
}

void User_validator::save_avatar_path(const QString& path)
{
    m_avatar_path = "file://" + path;
}
