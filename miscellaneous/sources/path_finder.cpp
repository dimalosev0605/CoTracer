#include "./miscellaneous/headers/path_finder.h"

QString Path_finder::get_app_dir_path() const
{
    return QCoreApplication::applicationDirPath() + '/';
}

QString Path_finder::get_path_to_user_files_dir() const
{
    return get_app_dir_path() + user_files_dir_name + '/';
}

QString Path_finder::get_path_to_user_avatar(bool prefix) const
{
    if(prefix) {
        return file_path_prefix + get_path_to_user_files_dir() + user_avatar_file_name;
    }
    else {
        return get_path_to_user_files_dir() + user_avatar_file_name;
    }
}

QString Path_finder::get_path_to_user_info_file() const
{
    return get_path_to_user_files_dir() + user_info_file_name;
}

QString Path_finder::get_path_to_avatars_dir() const
{
    return get_path_to_user_files_dir() + avatars_dir_name + '/';
}

QString Path_finder::get_path_to_particular_user_avatar(const QString& nickname, bool prefix) const
{
    if(prefix) {
        return file_path_prefix + get_path_to_avatars_dir() + nickname;
    }
    else {
        return get_path_to_avatars_dir() + nickname;
    }
}
