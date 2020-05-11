#include "file_dialog_model.h"

File_dialog_model::File_dialog_model(QObject *parent)
    : QAbstractListModel(parent)
{

    m_roles[(int)RolesNames::entry_name] = "entry_name";

    QDir root_dir(QDir::root());
    m_curr_abs_path = root_dir.absolutePath();
    m_entry_list = root_dir.entryList().toVector();
}

QHash<int, QByteArray> File_dialog_model::roleNames() const
{
    return m_roles;
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
        return m_entry_list[row];
        break;
    }

    }

    return QVariant();
}

bool File_dialog_model::try_cd(const QString& entry)
{
//    qDebug() << "Before cd: m_curr_abs_path = " << m_curr_abs_path;

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

//    qDebug() << "After cd: m_curr_abs_path = " << m_curr_abs_path;

    beginRemoveRows(QModelIndex(), 0, m_entry_list.size());
    m_entry_list.clear();
    endRemoveRows();

    QDir dir(m_curr_abs_path);
    auto new_entries = dir.entryList().toVector();
    if(new_entries.isEmpty()) return true;

    beginInsertRows(QModelIndex(), 0, new_entries.size() - 1);
    m_entry_list = new_entries;
    endInsertRows();

    return true;
}

void File_dialog_model::return_back()
{
    auto chars_for_remove = m_curr_abs_path.lastIndexOf("/");
    if(chars_for_remove < 0) return;

//    qDebug() << "start remove from: " << chars_for_remove;
    m_curr_abs_path = m_curr_abs_path.remove(chars_for_remove, m_curr_abs_path.size() - chars_for_remove);
    if(m_curr_abs_path.isEmpty()) {
        m_curr_abs_path = "/";
    }
//    qDebug() << "After removing m_curr_abs_path = " << m_curr_abs_path;

    beginRemoveRows(QModelIndex(), 0, m_entry_list.size());
    m_entry_list.clear();
    endRemoveRows();

    QDir dir(m_curr_abs_path);
    auto new_entries = dir.entryList().toVector();
    if(new_entries.isEmpty()) return;

    beginInsertRows(QModelIndex(), 0, new_entries.size() - 1);
    m_entry_list = new_entries;
    endInsertRows();
}

bool File_dialog_model::try_choose_photo(const QString& entry)
{
    qDebug() << entry;
    QString selected_photo_path = m_curr_abs_path + '/' + entry;
    qDebug() << "Selected path: " << selected_photo_path;

    emit photo("file://" + selected_photo_path);

    return true;
}
