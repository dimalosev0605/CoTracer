#include "./miscellaneous/headers/path_finder.h"

const QString Path_finder::user_files_dir_name = "user_files";
const QString Path_finder::user_avatar_file_name = "avatar";
const QString Path_finder::user_info_file_name = "user_info";
const QString Path_finder::avatars_dir_name = "avatars";
const QString Path_finder::file_path_prefix = "file://";

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
