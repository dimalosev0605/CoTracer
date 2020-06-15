#include "./models/headers/found_friends_model.h"

Found_friends_model::Found_friends_model(QObject *parent)
    : QAbstractListModel(parent)
{
    m_roles[(int)RolesNames::nickname] = "nickname";
    m_roles[(int)RolesNames::avatar_path] = "avatar_path";
}

QHash<int, QByteArray> Found_friends_model::roleNames() const
{
    return m_roles;
}

int Found_friends_model::rowCount(const QModelIndex&) const
{
    return m_found_friends.size();
}

QVariant Found_friends_model::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= m_found_friends.size()) return QVariant();

    switch (role) {

    case (int)RolesNames::nickname: {
        return m_found_friends[row];
    }

    case (int)RolesNames::avatar_path: {
        QString path = m_path_finder.get_path_to_particular_temp_file(m_found_friends[row], false);
        QFile file(path);
        if(file.exists()) {
            return m_path_finder.get_path_to_particular_temp_file(m_found_friends[row], true);
        }
        else {
            return m_path_finder.get_path_to_default_avatar_path();
        }
    }

    }

    return QVariant{};
}

void Found_friends_model::receive_found_friends(const QVector<QString> &found_friends)
{
    if(!m_found_friends.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, m_found_friends.size() - 1);
        m_found_friends.clear();
        endRemoveRows();
    }

    if(found_friends.isEmpty()) return;

    beginInsertRows(QModelIndex(), 0, found_friends.size() - 1);
    m_found_friends = found_friends;
    endInsertRows();
}
