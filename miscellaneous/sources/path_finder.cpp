#include "./miscellaneous/headers/path_finder.h"

const QString Path_finder::user_files_dir_name = "user_files";
const QString Path_finder::user_avatar_file_name = "avatar";
const QString Path_finder::user_info_file_name = "user_info";
const QString Path_finder::avatars_dir_name = "avatars";
const QString Path_finder::file_path_prefix = "file://";
const QString Path_finder::default_avatar_path = "qrc:/imgs/default_avatar.png";
const QString Path_finder::cached_avatars_info_file_name = "cached_avatars_info_file";
const QString Path_finder::temp_files_dir_name = "temp_files_dir";
const QString Path_finder::friends_avatar_dir_name = "friends_avatar_dir";
const QString Path_finder::friends_list_file_name = "friends_list";

QString Path_finder::get_app_dir_path() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + '/';
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

QString Path_finder::get_path_to_default_avatar_path() const
{
    return default_avatar_path;
}

QString Path_finder::get_path_to_cached_avatars_info_file() const
{
    return get_path_to_user_files_dir() + cached_avatars_info_file_name;
}

QString Path_finder::get_path_to_temp_files_dir() const
{
    return get_path_to_user_files_dir() + temp_files_dir_name + '/';
}

QString Path_finder::get_path_to_particular_temp_file(const QString& nick, bool prefix) const
{
    if(prefix) {
        return file_path_prefix + get_path_to_temp_files_dir() + nick;
    }
    else {
        return get_path_to_temp_files_dir() + nick;
    }
}

QString Path_finder::get_path_to_friends_avatar_dir() const
{
    return get_path_to_user_files_dir() + friends_avatar_dir_name + '/';
}

QString Path_finder::get_path_to_particular_friend_avatar(const QString& nick, bool prefix) const
{
    if(prefix) {
        return file_path_prefix + get_path_to_friends_avatar_dir() + nick;
    }
    else {
        return get_path_to_friends_avatar_dir() + nick;
    }
}

QString Path_finder::get_path_to_friends_list_file() const
{
    return get_path_to_user_files_dir() + friends_list_file_name;
}
