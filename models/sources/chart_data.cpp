#include "./models/headers/chart_data.h"

Chart_data::Chart_data(QObject *parent)
    : QObject(parent)
{}

void Chart_data::receive_stat(const QVector<std::tuple<QString, int>>& stat)
{
    if(stat.isEmpty()) return;
    m_data = stat;
    int max = std::get<1>(stat[0]);

    for(int i = 0; i < stat.size(); ++i) {
        QPoint p(i, std::get<1>(stat[i]));
        m_points.push_back(p);
        if(p.y() > max) {
            max = p.y();
        }
        emit point(p.x(), p.y());
    }

    emit set_max_count_of_contacts(max + 2);
}
