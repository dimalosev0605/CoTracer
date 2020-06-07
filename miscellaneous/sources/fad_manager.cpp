#include "./miscellaneous/headers/fad_manager.h"

FAD_manager::FAD_manager()
{
    create_user_files_dir();
    create_avatars_dir();
    create_cached_avatars_info_file();
}

bool FAD_manager::create_user_files_dir() const
{
    QDir dir(m_path_finder.get_app_dir_path());
    return dir.mkdir(m_path_finder.get_path_to_user_files_dir());
}

bool FAD_manager::create_avatars_dir() const
{
    QDir dir(m_path_finder.get_path_to_user_files_dir());
    return dir.mkdir(m_path_finder.get_path_to_avatars_dir());
}

bool FAD_manager::create_cached_avatars_info_file() const
{
    QFile file(m_path_finder.get_path_to_cached_avatars_info_file());
    if(!file.exists()) {
        if(file.open(QIODevice::WriteOnly)) {
            QJsonObject obj;
            obj.insert(Protocol_keys::cached_avatars, QJsonArray{});
            QJsonDocument doc(obj);
            file.write(doc.toJson());
            return true;
        }
        return false;
    }
    return true;
}

bool FAD_manager::delete_user_info_file() const
{
    QFile file(m_path_finder.get_path_to_user_info_file());
    return file.remove();
}

bool FAD_manager::delete_user_avatar_file() const
{
    QFile file(m_path_finder.get_path_to_user_avatar(false));
    return file.remove();
}

bool FAD_manager::delete_avatars_dir() const
{
    QDir dir(m_path_finder.get_path_to_avatars_dir());
    return dir.removeRecursively();
}

bool FAD_manager::delete_all_user_files() const
{
    if(delete_user_info_file()) {
        delete_avatars_dir_content();
        delete_user_avatar_file();
        delete_cached_avatars_info_file();
        return true;
    }
    return false;
}

void FAD_manager::delete_avatars_dir_content() const
{
    QDir dir(m_path_finder.get_path_to_avatars_dir());
    auto files = dir.entryList();
    for(const auto& i : files) {
        dir.remove(i);
    }
}

bool FAD_manager::delete_cached_avatars_info_file() const
{
    QFile file(m_path_finder.get_path_to_cached_avatars_info_file());
    return file.remove();
}
