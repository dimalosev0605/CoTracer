#include "./models/headers/contacts_model.h"

Contacts_model::Contacts_model(QObject* parent)
    : QAbstractListModel(parent)
{
    m_roles[(int)RolesNames::nickname] = "nickname";
    m_roles[(int)RolesNames::time] = "time";
    m_roles[(int)RolesNames::is_registered] = "is_registered";
    m_roles[(int)RolesNames::avatar_path] = "avatar_path";
}

Contacts_model::Contacts_model(const QVector<QString>& previous_links, QObject* parent)
    : QAbstractListModel(parent),
      m_previous_links(previous_links)
{
    m_roles[(int)RolesNames::nickname] = "nickname";
    m_roles[(int)RolesNames::time] = "time";
    m_roles[(int)RolesNames::is_registered] = "is_registered";
    m_roles[(int)RolesNames::avatar_path] = "avatar_path";

    qDebug() << "In model:";
    for(int i = 0; i < m_previous_links.size(); ++i) {
        qDebug() << m_previous_links[i];
    }
}

Contacts_model::~Contacts_model()
{
//    qDebug() << this << " destroyed";
}

QHash<int, QByteArray> Contacts_model::roleNames() const
{
    return m_roles;
}

int Contacts_model::rowCount(const QModelIndex& ) const
{
    return m_contacts.size();
}

QVariant Contacts_model::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= m_contacts.size()) return QVariant();


    switch (role) {

    case (int)RolesNames::nickname: {
        return std::get<0>(m_contacts[row]);
    }
    case (int)RolesNames::time: {
        return std::get<1>(m_contacts[row]);
    }
    case (int)RolesNames::is_registered: {
        if(std::find(m_previous_links.begin(), m_previous_links.end(), std::get<0>(m_contacts[row])) != m_previous_links.end()) {
            qDebug() << "Dont show: " << std::get<0>(m_contacts[row]);
            return false;
        }
        return std::get<2>(m_contacts[row]);
    }
    case (int)RolesNames::avatar_path: {
        QString path = "file://" + QCoreApplication::applicationDirPath() + '/' + "user_files" + '/' + "avatars" + '/' + std::get<0>(m_contacts[row]);
        qDebug() << "path = " << path;
        return path;
    }

    }

    return QVariant();
}

void Contacts_model::remove_contact(int index)
{
    qDebug() << "remove contact: " << index;
    if(index < 0 || index >= m_contacts.size()) return;
    beginRemoveRows(QModelIndex(), index, index);
    m_contacts.removeAt(index);
    endRemoveRows();
}

void Contacts_model::add_contact(const QString& nickname, const QString& time, bool is_reg)
{
    beginInsertRows(QModelIndex(), m_contacts.size(), m_contacts.size());
    m_contacts.push_back(std::make_tuple(nickname, time, is_reg));
    endInsertRows();
}

void Contacts_model::receive_contacts(const QVector<std::tuple<QString, QString, bool>>& contacts)
{
    if(contacts.isEmpty()) return;

    beginInsertRows(QModelIndex(), 0, contacts.size() - 1);
    m_contacts = contacts;

    std::sort(m_contacts.begin(), m_contacts.end(), [](const std::tuple<QString, QString, bool> lhs,
                                                       const std::tuple<QString, QString, bool> rhs)
    {
        auto lhs_time = QTime::fromString(std::get<1>(lhs), "hh:mm");
        auto rhs_time = QTime::fromString(std::get<1>(rhs), "hh:mm");
        return lhs_time < rhs_time;
    });


    endInsertRows();
}

