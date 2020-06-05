#include "./miscellaneous/headers/fad_manager.h"

FAD_manager::FAD_manager()
{
    create_user_files_dir();
    create_avatars_dir();
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
    delete_avatars_dir_content();
    delete_user_avatar_file();
    return delete_user_info_file();
}

void FAD_manager::delete_avatars_dir_content() const
{
    QDir dir(m_path_finder.get_path_to_avatars_dir());
    auto files = dir.entryList();
    for(const auto& i : files) {
        dir.remove(i);
    }
}
