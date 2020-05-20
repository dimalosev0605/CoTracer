#ifndef CHART_DATA_H
#define CHART_DATA_H

#include <QObject>
#include <QDebug>
#include <QPoint>

class Chart_data : public QObject
{
    Q_OBJECT

    QVector<QPoint> m_points_sum;
    QVector<QPoint> m_points_reg;
    QVector<QPoint> m_points_unreg;

public:
    explicit Chart_data(QObject *parent = nullptr);

public slots:
    void receive_stats(const QVector<std::tuple<int, int>>& stats);

signals:
    void sum_point(int x, int y);
    void reg_point(int x, int y);
    void unreg_point(int x, int y);
    void set_max(int max);
};

#endif // CHART_DATA_H
