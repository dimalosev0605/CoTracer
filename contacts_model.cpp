#include "contacts_model.h"

Contacts_model::Contacts_model(const QString& name, QObject* parent)
    : QAbstractListModel(parent),
      m_host_name(name)
{
    m_roles[(int)RolesNames::nickname] = "nickname";
    m_roles[(int)RolesNames::time] = "time";
    m_roles[(int)RolesNames::is_registered] = "is_registered";
//    qDebug() << this << " created";
}

Contacts_model::~Contacts_model()
{
//    qDebug() << this << " destroyed";
}

QHash<int, QByteArray> Contacts_model::roleNames() const
{
    return m_roles;
}

int Contacts_model::rowCount(const QModelIndex &index) const
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
//        qDebug() << std::get<0>(m_contacts[row]) << " - " << std::get<2>(m_contacts[row]);
//        qDebug() << "m_my_name = " << m_host_name;
        if(m_host_name == std::get<0>(m_contacts[row])) {
            qDebug() << "LALAALALALALALALALALLAAL";
            return false;
        }
        return std::get<2>(m_contacts[row]);
    }

    }

    return QVariant();
}

void Contacts_model::receive_unregistered_contacts(const QVector<std::pair<QString, QString>>& contacts)
{
    for(int i = 0; i < contacts.size(); ++i) {
        beginInsertRows(QModelIndex(), m_contacts.size(), m_contacts.size());
        m_contacts.push_back(std::make_tuple(contacts[i].first, contacts[i].second, false));
        endInsertRows();
    }
    qDebug() << "Unreg contacts:";
    for(auto& i : contacts) {
        qDebug() << i.first << " - " << i.second;
    }
}

void Contacts_model::receive_registered_contacts(const QVector<std::pair<QString, QString>>& contacts)
{

    for(int i = 0; i < contacts.size(); ++i) {
        beginInsertRows(QModelIndex(), m_contacts.size(), m_contacts.size());
        m_contacts.push_back(std::make_tuple(contacts[i].first, contacts[i].second, true));
        endInsertRows();
    }

    qDebug() << "Reg contacts:";
    for(auto& i : contacts) {
        qDebug() << i.first << " - " << i.second;
    }
}

void Contacts_model::remove_contact(int index)
{
    if(index < 0 || index >= m_contacts.size()) return;
    beginRemoveRows(QModelIndex(), index, index);
    m_contacts.removeAt(index);
    endRemoveRows();
}
