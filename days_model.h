#ifndef DAYS_MODEL_H
#define DAYS_MODEL_H

#include <QAbstractListModel>
#include <QDate>
#include <QDebug>
#include <QThread>
#include <QTimer>

class Days_model: public QAbstractListModel
{
    Q_OBJECT

    static const int count_of_dates = 14;

    QHash<int, QByteArray> m_roles;
//    QVector<QString> m_dates;
    QVector<std::tuple<QString, int, int>> m_stats;

    bool m_date_sorted_decrease_order = 1;
    bool m_reg_count_sorted_decrease_order = 1;
    bool m_unreg_count_sorted_decrease_order = 1;
    bool m_sum_sorted_decrease_order = 1;

private:
    QHash<int, QByteArray> roleNames() const override;
    void fill_m_dates();

public:
    enum class RolesNames {
        date = Qt::UserRole,
        reg_count = Qt::UserRole + 1,
        unreg_count = Qt::UserRole + 2
    };

    Days_model(QObject* parent = nullptr);
    virtual int rowCount(const QModelIndex &index = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

public slots:
    void receive_stats(const QVector<std::tuple<QString, int, int>>& stats);
    void sort_by_date();
    void sort_by_reg_count();
    void sort_by_unreg_count();
    void sort_by_sum();
    QVector<std::tuple<int, int>> get_stats();
};

#endif // DAYS_MODEL_H
