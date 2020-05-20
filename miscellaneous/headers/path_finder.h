#ifndef PATH_FINDER_H
#define PATH_FINDER_H

#include <QCoreApplication>
#include <QDebug>

class Path_finder
{
    // make static?
    const QString user_files_dir_name = "user_files";
    const QString user_avatar_file_name = "avatar";
    const QString user_info_file_name = "user_info";
    const QString avatars_dir_name = "avatars";
    const QString file_path_prefix = "file://";

private:

public:
    QString get_app_dir_path() const;
    QString get_path_to_user_files_dir() const;
    QString get_path_to_user_avatar(bool prefix) const;
    QString get_path_to_user_info_file() const;
    QString get_path_to_avatars_dir() const;
    QString get_path_to_particular_user_avatar(const QString& nickname, bool prefix) const;
};

#endif // PATH_FINDER_H
