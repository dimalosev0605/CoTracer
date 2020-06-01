#include "./models/headers/stat_model.h"

Stat_model::Stat_model(QObject* parent)
    : QAbstractListModel(parent)
{
    m_roles[(int)RolesNames::date] = "date";
    m_roles[(int)RolesNames::count_of_contacts] = "count_of_contacts";
}

QHash<int, QByteArray> Stat_model::roleNames() const
{
    return m_roles;
}

int Stat_model::rowCount(const QModelIndex&) const
{
    return m_stat.size();
}

QVariant Stat_model::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= m_stat.size()) return QVariant();

    switch (role) {

    case (int)RolesNames::date: {
        return std::get<0>(m_stat[row]);
        break;
    }
    case (int)RolesNames::count_of_contacts: {
        return std::get<1>(m_stat[row]);
        break;
    }

    }

    return QVariant();
}

void Stat_model::receive_stats(const QVector<std::tuple<QString, int>>& statistic)
{
    if(statistic.isEmpty()) return;

    if(!m_stat.isEmpty()) {
        beginRemoveRows(QModelIndex(), 0, m_stat.size() - 1);
        m_stat.clear();
        endRemoveRows();
    }

    beginInsertRows(QModelIndex(), 0, statistic.size() - 1);
    m_stat = statistic;
    std::sort(m_stat.begin(), m_stat.end(), [](const std::tuple<QString, int>& lhs, const std::tuple<QString, int>& rhs)
    {
        QDate lhs_date = QDate::fromString(std::get<0>(lhs), "dd.MM.yy");
        QDate rhs_date = QDate::fromString(std::get<0>(rhs), "dd.MM.yy");
        return lhs_date > rhs_date;
    });
    endInsertRows();
}
