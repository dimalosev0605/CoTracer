#include "contacts_model.h"

Contacts_model::Contacts_model(QObject* parent)
    : QAbstractListModel(parent)
{
    m_roles[(int)RolesNames::nickname] = "nickname";
    m_roles[(int)RolesNames::time] = "time";
    m_roles[(int)RolesNames::is_registered] = "is_registered";
    qDebug() << this << " created";
}

Contacts_model::~Contacts_model()
{
    qDebug() << this << " destroyed";
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
        return std::get<2>(m_contacts[row]);
    }

    }

    return QVariant();
}

void Contacts_model::receive_unregistered_contacts(const QVector<std::pair<QString, QString>>& contacts)
{
//    qDebug() << "Unregistered Contacts in model:";
//    for(auto& i : contacts) {
//        qDebug() << i.first << " - " << i.second;
//    }

    for(int i = 0; i < contacts.size(); ++i) {
        beginInsertRows(QModelIndex(), m_contacts.size(), m_contacts.size());
        m_contacts.push_back(std::make_tuple(contacts[i].first, contacts[i].second, false));
        endInsertRows();
    }
}

void Contacts_model::receive_registered_contacts(const QVector<std::pair<QString, QString>>& contacts)
{

    for(int i = 0; i < contacts.size(); ++i) {
        beginInsertRows(QModelIndex(), m_contacts.size(), m_contacts.size());
        m_contacts.push_back(std::make_tuple(contacts[i].first, contacts[i].second, true));
        endInsertRows();
    }

    qDebug() << "All contacts in model:";
    for(auto& i : m_contacts) {
        qDebug() << std::get<0>(i) << " - " << std::get<1>(i);
    }
}
