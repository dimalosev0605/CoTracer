#include "./models/headers/my_friends_model.h"

My_friends_model::My_friends_model(QObject *parent)
    : QAbstractListModel(parent)
{

    m_roles[(int)RolesNames::friend_nickname] = "friend_nickname";
    m_roles[(int)RolesNames::friend_avatar_path] = "friend_avatar_path";
    m_roles[(int)RolesNames::delegate_type] = "delegate_type";

    load_friends_list();

    m_sorted_friends = m_friends;

    sort_friends_list();
}

My_friends_model::~My_friends_model()
{
    save_friends_list();
}

QHash<int, QByteArray> My_friends_model::roleNames() const
{
    return m_roles;
}

int My_friends_model::rowCount(const QModelIndex& ) const
{
    return m_sorted_friends.size();
}

QVariant My_friends_model::data(const QModelIndex &index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= m_sorted_friends.size()) return QVariant();

    switch (role) {

    case (int)RolesNames::friend_nickname: {
        return m_sorted_friends[row];
    }

    case (int)RolesNames::friend_avatar_path: {
        QFile file(m_path_finder.get_path_to_particular_friend_avatar(m_sorted_friends[row], false));
        if(file.exists()) {
            return m_path_finder.get_path_to_particular_friend_avatar(m_sorted_friends[row], true);
        }
        else {
            return m_path_finder.get_path_to_default_avatar_path();
        }
    }

    case (int)RolesNames::delegate_type: {
        if(m_sorted_friends[row].size() == 1) {
            return false;
        }
        return true;
    }

    }

    return QVariant{};
}

void My_friends_model::add_friend(const QString& new_friend)
{
    auto iter = std::find(m_friends.begin(), m_friends.end(), new_friend);
    if(iter != m_friends.end()) {
        return;
    }

    if(!m_sorted_friends.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, m_sorted_friends.size() - 1);
        endRemoveRows();
    }

    m_friends.push_back(new_friend);
    m_sorted_friends.push_back(new_friend);
    sort_friends_list();
    if(!m_sorted_friends.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, m_sorted_friends.size() - 1);
        endInsertRows();
    }
}

void My_friends_model::load_friends_list()
{
    QFile friends_list_file(m_path_finder.get_path_to_friends_list_file());
    if(friends_list_file.open(QIODevice::ReadOnly)) {
        auto doc = QJsonDocument::fromJson(friends_list_file.readAll());
        if(!doc.isEmpty()) {
            auto obj = doc.object();
            auto map = obj.toVariantMap();
            auto f_list = map[Protocol_keys::friends_list].toJsonArray();
            for(int i = 0; i < f_list.size(); ++i) {
                auto obj = f_list[i].toObject();
                auto map = obj.toVariantMap();
                QString nick = map[Protocol_keys::friend_nickname].toString();
                m_friends.push_back(nick);
            }
        }
    }
}

void My_friends_model::save_friends_list()
{
    QFile friends_list_file(m_path_finder.get_path_to_friends_list_file());
    if(friends_list_file.open(QIODevice::WriteOnly)) {
        QJsonArray arr;
        for(int i = 0; i < m_friends.size(); ++i) {
            QJsonObject obj;
            obj.insert(Protocol_keys::friend_nickname, m_friends[i]);
            arr.append(obj);
        }
        QJsonObject j_obj;
        j_obj.insert(Protocol_keys::friends_list, arr);
        QJsonDocument doc(j_obj);
        friends_list_file.write(doc.toJson());
    }
}

void My_friends_model::sort_friends_list()
{
    for(int i = 0; i < m_sorted_friends.size(); ++i) {
        if(m_sorted_friends[i].size() == 1) {
            m_sorted_friends[i] = "";
        }
    }
    m_sorted_friends.removeAll("");

    QVector<QChar> temp;
    for(int i = 0; i < m_sorted_friends.size(); ++i) {
        if(!temp.contains(m_sorted_friends[i].front())) {
            temp.push_back(m_sorted_friends[i].front());
        }
    }

    for(int i = 0; i < temp.size(); ++i) {
        m_sorted_friends.push_back(QString{temp[i]});
    }

    std::sort(m_sorted_friends.begin(), m_sorted_friends.end());
}

void My_friends_model::remove_friend(const QString& nickname)
{
    if(!m_sorted_friends.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, m_sorted_friends.size() - 1);
        endRemoveRows();
    }

    auto iter = std::find(m_friends.begin(), m_friends.end(), nickname);
    if(iter != m_friends.end()) {
        m_friends.erase(iter);
        save_friends_list();
    }

    auto another_iter = std::find(m_sorted_friends.begin(), m_sorted_friends.end(), nickname);
    if(another_iter != m_sorted_friends.end()) {
        m_sorted_friends.erase(another_iter);
    }
    sort_friends_list();
    if(!m_sorted_friends.isEmpty()) {
        beginInsertRows(QModelIndex(), 0, m_sorted_friends.size() - 1);
        endInsertRows();
    }
}
