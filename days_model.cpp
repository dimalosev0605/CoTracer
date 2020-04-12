#include "days_model.h"

Days_model::Days_model(QObject* parent)
    : QAbstractListModel(parent)
{
    m_roles[(int)RolesNames::date] = "date";
    fill_m_dates();
}

QHash<int, QByteArray> Days_model::roleNames() const
{
    return m_roles;
}

int Days_model::rowCount(const QModelIndex&) const
{
    return m_dates.size();
}

QVariant Days_model::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= m_dates.size()) return QVariant();

    switch (role) {

    case (int)RolesNames::date: {
        return m_dates[row];
        break;
    }

    }

    return QVariant();
}

void Days_model::fill_m_dates()
{
    m_dates.reserve(count_of_dates);
    auto today = QDate(QDate::currentDate());
    for(int i = 0; i < count_of_dates; ++i) {
        m_dates.push_back(today.toString("dd.MM.yy"));
        today = today.addDays(-1);
    }
}

Contacts_model* Days_model::create_model()
{
    Contacts_model* p = new Contacts_model;
    m_models.push_back(p);
    return p;
}

void Days_model::destroy_model()
{
    auto temp = m_models.last();
    m_models.pop_back();
    delete temp;
}
