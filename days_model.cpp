#include "days_model.h"

Days_model::Days_model(QObject* parent)
    : QAbstractListModel(parent)
{
    m_roles[(int)RolesNames::date] = "date";
    m_roles[(int)RolesNames::reg_count] = "reg_count";
    m_roles[(int)RolesNames::unreg_count] = "unreg_count";
//    fill_m_dates();
}

QHash<int, QByteArray> Days_model::roleNames() const
{
    return m_roles;
}

int Days_model::rowCount(const QModelIndex&) const
{
//    return m_dates.size();
    return m_stats.size();
}

QVariant Days_model::data(const QModelIndex& index, int role) const
{
    int row = index.row();
    if(row < 0 || row >= m_stats.size()) return QVariant();

    switch (role) {

    case (int)RolesNames::date: {
        return std::get<0>(m_stats[row]);
        break;
    }
    case (int)RolesNames::reg_count: {
        return std::get<1>(m_stats[row]);
        break;
    }
    case (int)RolesNames::unreg_count: {
        return std::get<2>(m_stats[row]);
        break;
    }

    }

    return QVariant();
}

void Days_model::fill_m_dates()
{
//    m_dates.reserve(count_of_dates);
//    auto today = QDate(QDate::currentDate());
//    for(int i = 0; i < count_of_dates; ++i) {
//        m_dates.push_back(today.toString("dd.MM.yy"));
//        today = today.addDays(-1);
//    }
}

void Days_model::receive_stats(const QVector<std::tuple<QString, int, int>>& stats)
{
    if(stats.isEmpty()) return;
    beginInsertRows(QModelIndex(), 0, stats.size() - 1);
    m_stats = stats;
    std::sort(m_stats.begin(), m_stats.end(), [](const std::tuple<QString, int, int>& lhs,
                                                 const std::tuple<QString, int, int>& rhs)
    {
        return std::get<0>(lhs) > std::get<0>(rhs);
    });
    endInsertRows();
}
