#include "chart_data.h"

Chart_data::Chart_data(QObject *parent) : QObject(parent)
{

}

void Chart_data::receive_stats(const QVector<std::tuple<int, int>>& stats)
{
    for(int i = 0; i < stats.size(); ++i) {
        m_points_sum.push_back(QPoint(i, std::get<0>(stats[i]) + std::get<1>(stats[i])));
        m_points_reg.push_back(QPoint(i, std::get<0>(stats[i])));
        m_points_unreg.push_back(QPoint(i, std::get<1>(stats[i])));

        emit sum_point(i, m_points_sum[i].y());
        emit reg_point(i, m_points_reg[i].y());
        emit unreg_point(i, m_points_unreg[i].y());
    }

    int max = m_points_sum.first().y();
    for(int i = 1; i < m_points_sum.size(); ++i) {
        if(m_points_sum[i].y() > max) {
            max = m_points_sum[i].y();
        }
    }
    int max_value_on_y_axis = max + 1;
    emit set_max(max_value_on_y_axis);
}
