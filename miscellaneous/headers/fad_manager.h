#ifndef FAD_MANAGER_H
#define FAD_MANAGER_H

#include <QDir>

#include "path_finder.h"

class FAD_manager
{
    Path_finder m_path_finder;

private:

public:
    FAD_manager();

    bool create_user_files_dir() const;
    bool create_avatars_dir() const;

    bool delete_user_info_file() const;
    bool delete_user_avatar_file() const;
    bool delete_avatars_dir() const;
    bool delete_all_user_files() const;
    void delete_avatars_dir_content() const;
};

#endif // FAD_MANAGER_H
