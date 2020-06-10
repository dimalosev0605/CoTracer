#ifndef PATH_FINDER_H
#define PATH_FINDER_H

#include <QCoreApplication>
#include <QDebug>
#include <QStandardPaths>

class Path_finder
{
    static const QString user_files_dir_name;
    static const QString user_avatar_file_name;
    static const QString user_info_file_name;
    static const QString avatars_dir_name;
    static const QString file_path_prefix;
    static const QString default_avatar_path;
    static const QString cached_avatars_info_file_name;

public:
    QString get_app_dir_path() const;
    QString get_path_to_user_files_dir() const;
    QString get_path_to_user_avatar(bool prefix) const;
    QString get_path_to_user_info_file() const;
    QString get_path_to_avatars_dir() const;
    QString get_path_to_particular_user_avatar(const QString& nickname, bool prefix) const;
    QString get_path_to_default_avatar_path() const;
    QString get_path_to_cached_avatars_info_file() const;
};

#endif // PATH_FINDER_H
