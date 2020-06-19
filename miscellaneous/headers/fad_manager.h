#ifndef FAD_MANAGER_H
#define FAD_MANAGER_H

#include <QDir>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "path_finder.h"
#include "networking/headers/Protocol_keys.h"

class FAD_manager
{
    Path_finder m_path_finder;

private:

public:
    FAD_manager();

    bool create_user_files_dir() const;
    bool create_avatars_dir() const;
    bool create_cached_avatars_info_file() const;
    bool create_temp_files_dir() const;
    bool create_friends_avatar_dir() const;

    bool delete_user_info_file() const;
    bool delete_user_avatar_file() const;
    bool delete_avatars_dir() const;
    bool delete_all_user_files() const;
    void delete_avatars_dir_content() const;
    bool delete_cached_avatars_info_file() const;
    void delete_all_temp_files() const;
    void delete_friends_avatar_dir_content() const;
};

#endif // FAD_MANAGER_H
