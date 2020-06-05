#ifndef CHART_DATA_H
#define CHART_DATA_H

#include <QObject>
#include <QDebug>
#include <QPoint>

class Chart_data : public QObject
{
    Q_OBJECT

    QVector<QPoint> m_points;
    QVector<std::tuple<QString, int>> m_data;

public:
    explicit Chart_data(QObject *parent = nullptr);

public slots:
    void receive_stat(const QVector<std::tuple<QString, int>>& stat);

signals:
    void point(int x, int y);
    void set_max_count_of_contacts(int max_value);
};

#endif // CHART_DATA_H
