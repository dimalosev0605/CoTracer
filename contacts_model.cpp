#include "contacts_model.h"

Contacts_model::Contacts_model(QObject* parent)
    : QAbstractListModel(parent)
{
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
    return QVariant();
}
