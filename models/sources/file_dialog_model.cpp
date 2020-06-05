#include "./models/headers/file_dialog_model.h"

const QString date_format = "dd/MM/yy  hh:mm:ss";
const QString folder_img_path = "qrc:/imgs/folder.png";
const QString file_prefix = "file://";

File_dialog_model::File_dialog_model(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roles[(int)RolesNames::name] = "name";
    m_roles[(int)RolesNames::last_modified_time] = "last_modified_time";
    m_roles[(int)RolesNames::path_to_img] = "path_to_img";

    m_dir = QDir::home();
    set_curr_abs_path(m_dir.absolutePath());

    QStringList filters;
    filters << "*.png" << "*.jpg" << "*.jpeg";
    m_dir.setNameFilters(filters);
    m_dir.setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);

    update_content();
}

QHash<int, QByteArray> File_dialog_model::roleNames() const
{
    return m_roles;
}

void File_dialog_model::clear()
{
    if(m_data.isEmpty()) return;
    beginRemoveRows(QModelIndex(), 0, m_data.size() - 1);
    m_data.clear();
    endRemoveRows();
}

void File_dialog_model::update_content(QDir::SortFlag sort_flag)
{
    m_dir.setPath(m_curr_abs_path);
    m_dir.setSorting(sort_flag);
    m_dir.refresh();
    clear();

    auto new_entries = m_dir.entryInfoList().toVector();
    if(new_entries.isEmpty()) return;

    beginInsertRows(QModelIndex(), 0, new_entries.size() - 1);
    auto temp = m_dir.entryInfoList().toVector();
    for(int i = 0; i < temp.size(); ++i) {
        bool is_dir = temp[i].isDir();
        QString path;
        if(is_dir) {
            path = folder_img_path;
        }
        else {
            path = file_prefix + temp[i].filePath();
        }
        m_data.push_back(std::make_tuple(temp[i].fileName(), temp[i].lastModified().toString(date_format), path));
    }
    endInsertRows();
}

void File_dialog_model::set_curr_abs_path(const QString& path)
{
    m_curr_abs_path = path;
    emit current_path_changed(path);
}

int File_dialog_model::rowCount(const QModelIndex& ) const
{
    return m_data.size();
}

QVariant File_dialog_model::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= m_data.size()) return QVariant();

    switch (role) {

    case (int)RolesNames::name: {
        return std::get<0>(m_data[row]);
    }
    case (int)RolesNames::last_modified_time: {
        return std::get<1>(m_data[row]);
    }
    case (int)RolesNames::path_to_img: {
        return std::get<2>(m_data[row]);
    }

    }

    return QVariant();
}

bool File_dialog_model::cd(const QString& directory)
{
    QFileInfo is_root(m_curr_abs_path);
    bool is_in_root;
    QFileInfo is_dir;

    if(is_root.isRoot()) {
        is_in_root = true;
        is_dir.setFile(m_curr_abs_path + directory);
    }
    else {
        is_in_root = false;
        is_dir.setFile(m_curr_abs_path + '/' + directory);
    }

    if(is_dir.isDir()) {
        if(is_in_root) {
            QString path = m_curr_abs_path + directory;
            set_curr_abs_path(path);
        }
        else {
            QString path = m_curr_abs_path + '/' + directory;
            set_curr_abs_path(path);
        }
    }
    else {
        return false;
    }

    update_content();

    return true;
}

void File_dialog_model::return_in_parent_directory()
{
    auto chars_for_remove = m_curr_abs_path.lastIndexOf("/");
    if(chars_for_remove < 0) return;

    QString path = m_curr_abs_path.remove(chars_for_remove, m_curr_abs_path.size() - chars_for_remove);
    set_curr_abs_path(path);

    if(m_curr_abs_path.isEmpty()) {
        set_curr_abs_path("/");
    }

    update_content();
}

bool File_dialog_model::choose_avatar(const QString& entry)
{
    QFileInfo is_root(m_curr_abs_path);
    QString selected_avatar_path;
    if(is_root.isRoot()) {
        selected_avatar_path = m_curr_abs_path + entry;
        emit selected(file_prefix + selected_avatar_path);
    }
    else {
        selected_avatar_path = m_curr_abs_path + '/' + entry;
        emit selected(file_prefix + selected_avatar_path);
    }
    return true;
}

void File_dialog_model::refresh()
{
    update_content();
}

QString File_dialog_model::get_current_path() const
{
    return m_curr_abs_path;
}

void File_dialog_model::search(const QString& input)
{
    if(input.isEmpty()) {
        clear();
        return;
    }

    QVector<int> indexes;
    for(int i = 0; i < m_data_copy.size(); ++i) {
        QString name = std::get<0>(m_data_copy[i]);
        if(name.contains(input, Qt::CaseInsensitive)) {
            indexes.push_back(i);
        }
    }

    clear();

    if(indexes.size()) {
        beginInsertRows(QModelIndex(), 0, indexes.size() - 1);
        for(int i = 0; i < indexes.size(); ++i) {
            m_data.push_back(m_data_copy[indexes[i]]);
        }
        endInsertRows();
    }
}

void File_dialog_model::prepare_search()
{
    if(m_data.isEmpty()) return;
    beginRemoveRows(QModelIndex(), 0, m_data.size() - 1);
    m_data_copy = std::move(m_data);
    endRemoveRows();
}

void File_dialog_model::cancel_search()
{
    m_data_copy.clear();
    update_content();
}

void File_dialog_model::sort_by_time()
{
    update_content(QDir::SortFlag::Time);
}

void File_dialog_model::sort_by_name()
{
    update_content(QDir::SortFlag::Name);
}
