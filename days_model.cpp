#include "days_model.h"

Days_model::Days_model(QObject* parent)
    : QAbstractListModel(parent)
{
    m_roles[(int)RolesNames::date] = "date";
    m_roles[(int)RolesNames::reg_count] = "reg_count";
    m_roles[(int)RolesNames::unreg_count] = "unreg_count";
}

QHash<int, QByteArray> Days_model::roleNames() const
{
    return m_roles;
}

int Days_model::rowCount(const QModelIndex&) const
{
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


void Days_model::receive_stats(const QVector<std::tuple<QString, int, int>>& stats)
{
    if(stats.isEmpty()) return;
    beginInsertRows(QModelIndex(), 0, stats.size() - 1);
    m_stats = stats;
    std::sort(m_stats.begin(), m_stats.end(), [](const std::tuple<QString, int, int>& lhs,
                                                 const std::tuple<QString, int, int>& rhs)
    {
        QDate lhs_date = QDate::fromString(std::get<0>(lhs), "dd.MM.yy");
        QDate rhs_date = QDate::fromString(std::get<0>(rhs), "dd.MM.yy");
        return lhs_date > rhs_date;
    });
    endInsertRows();
}

void Days_model::sort_by_date()
{
    beginRemoveRows(QModelIndex(), 0, m_stats.size() - 1);
    endRemoveRows();

    beginInsertRows(QModelIndex(), 0, m_stats.size() - 1);
    std::sort(m_stats.begin(), m_stats.end(), [this](const std::tuple<QString, int, int>& lhs,
                                                     const std::tuple<QString, int, int>& rhs)
    {
        QDate lhs_date = QDate::fromString(std::get<0>(lhs), "dd.MM.yy");
        QDate rhs_date = QDate::fromString(std::get<0>(rhs), "dd.MM.yy");
        if(m_date_sorted_decrease_order) {
            return lhs_date < rhs_date;
        } else {
            return lhs_date > rhs_date;
        }
    });
    endInsertRows();
    m_date_sorted_decrease_order = !m_date_sorted_decrease_order;
}

void Days_model::sort_by_reg_count()
{
    beginRemoveRows(QModelIndex(), 0, m_stats.size() - 1);
    endRemoveRows();

    beginInsertRows(QModelIndex(), 0, m_stats.size() - 1);
    std::sort(m_stats.begin(), m_stats.end(), [this](const std::tuple<QString, int, int>& lhs,
                                                     const std::tuple<QString, int, int>& rhs)
    {
        if(m_reg_count_sorted_decrease_order) {
            return std::get<1>(lhs) < std::get<1>(rhs);
        } else {
            return std::get<1>(lhs) > std::get<1>(rhs);
        }
    });
    endInsertRows();
    m_reg_count_sorted_decrease_order = !m_reg_count_sorted_decrease_order;
}

void Days_model::sort_by_unreg_count()
{
    beginRemoveRows(QModelIndex(), 0, m_stats.size() - 1);
    endRemoveRows();

    beginInsertRows(QModelIndex(), 0, m_stats.size() - 1);
    std::sort(m_stats.begin(), m_stats.end(), [this](const std::tuple<QString, int, int>& lhs,
                                                     const std::tuple<QString, int, int>& rhs)
    {
        if(m_unreg_count_sorted_decrease_order) {
            return std::get<2>(lhs) < std::get<2>(rhs);
        } else {
            return std::get<2>(lhs) > std::get<2>(rhs);
        }
    });
    endInsertRows();
    m_unreg_count_sorted_decrease_order = !m_unreg_count_sorted_decrease_order;
}

void Days_model::sort_by_sum()
{
    beginRemoveRows(QModelIndex(), 0, m_stats.size() - 1);
    endRemoveRows();

    beginInsertRows(QModelIndex(), 0, m_stats.size() - 1);
    std::sort(m_stats.begin(), m_stats.end(), [this](const std::tuple<QString, int, int>& lhs,
                                                     const std::tuple<QString, int, int>& rhs)
    {
        int lhs_sum = std::get<1>(lhs) + std::get<2>(lhs);
        int rhs_sum = std::get<1>(rhs) + std::get<2>(rhs);
        if(m_sum_sorted_decrease_order) {
            return lhs_sum < rhs_sum;
        } else {
            return lhs_sum > rhs_sum;
        }
    });
    endInsertRows();
    m_sum_sorted_decrease_order = !m_sum_sorted_decrease_order;
}

QVector<std::tuple<int, int>> Days_model::get_stats()
{
    auto temp = m_stats;

    std::sort(temp.begin(), temp.end(), [](const std::tuple<QString, int, int>& lhs,
                                           const std::tuple<QString, int, int>& rhs)
    {
        return std::get<0>(lhs) > std::get<0>(rhs);
    });

    QVector<std::tuple<int, int>> stats;
    for(int i = 0; i < temp.size(); ++i) {
        stats.push_back(std::make_tuple(std::get<1>(temp[i]), std::get<2>(temp[i])));
    }

    return stats;
}
