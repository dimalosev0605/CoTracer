#include "./models/headers/file_dialog_model.h"

File_dialog_model::File_dialog_model(QObject *parent)
    : QAbstractListModel(parent)
{

    m_roles[(int)RolesNames::entry_name] = "entry_name";
    m_roles[(int)RolesNames::is_dir_or_file] = "is_dir_or_file";

    m_filters << "*.png" << "*.jpg" << "*.jpeg";

    m_dir = QDir::home();
    m_curr_abs_path = m_dir.absolutePath();

    m_dir.setNameFilters(m_filters);
    m_dir.setFilter(QDir::Files | QDir::AllDirs | QDir::NoDotAndDotDot);

    auto temp = m_dir.entryInfoList().toVector();
    for(int i = 0; i < temp.size(); ++i) {
        m_entry_list.push_back(std::make_tuple(temp[i].fileName(), temp[i].isDir()));
    }
}

QHash<int, QByteArray> File_dialog_model::roleNames() const
{
    return m_roles;
}

void File_dialog_model::clear()
{
    beginRemoveRows(QModelIndex(), 0, m_entry_list.size());
    m_entry_list.clear();
    endRemoveRows();
}

void File_dialog_model::insert()
{
    m_dir.setPath(m_curr_abs_path);
    auto new_entries = m_dir.entryList().toVector();
    if(new_entries.isEmpty()) return;

    beginInsertRows(QModelIndex(), 0, new_entries.size() - 1);
    auto temp = m_dir.entryInfoList().toVector();
    for(int i = 0; i < temp.size(); ++i) {
        m_entry_list.push_back(std::make_tuple(temp[i].fileName(), temp[i].isDir()));
    }
    endInsertRows();
}

int File_dialog_model::rowCount(const QModelIndex& ) const
{
    return m_entry_list.size();
}

QVariant File_dialog_model::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= m_entry_list.size()) return QVariant();

    switch (role) {

    case (int)RolesNames::entry_name: {
        return std::get<0>(m_entry_list[row]);
        break;
    }

    case (int)RolesNames::is_dir_or_file: {
        return std::get<1>(m_entry_list[row]);
    }

    }

    return QVariant();
}

bool File_dialog_model::try_cd(const QString& entry)
{
    QFileInfo is_root(m_curr_abs_path);
    bool is_in_root;
    QFileInfo is_dir;

    if(is_root.isRoot()) {
        is_in_root = true;
        is_dir.setFile(m_curr_abs_path + entry);
    }
    else {
        is_in_root = false;
        is_dir.setFile(m_curr_abs_path + '/' + entry);
    }

    if(is_dir.isDir()) {
        if(is_in_root) {
            m_curr_abs_path += entry;
        }
        else {
            m_curr_abs_path += '/' + entry;
        }
    }
    else {
        return false;
    }

    clear();
    insert();

    return true;
}

void File_dialog_model::return_back()
{
    auto chars_for_remove = m_curr_abs_path.lastIndexOf("/");
    if(chars_for_remove < 0) return;

    m_curr_abs_path = m_curr_abs_path.remove(chars_for_remove, m_curr_abs_path.size() - chars_for_remove);
    if(m_curr_abs_path.isEmpty()) {
        m_curr_abs_path = "/";
    }

    clear();
    insert();
}

bool File_dialog_model::try_choose_avatar(const QString& entry)
{
    QFileInfo is_root(m_curr_abs_path);
    QString selected_avatar_path;
    if(is_root.isRoot()) {
        selected_avatar_path = m_curr_abs_path + entry;
        emit selected("file://" + selected_avatar_path);
    }
    else {
        selected_avatar_path = m_curr_abs_path + '/' + entry;
        emit selected("file://" + selected_avatar_path);
    }
    return true;
}

void File_dialog_model::refresh()
{
    clear();
    m_dir.refresh();
    insert();
}
