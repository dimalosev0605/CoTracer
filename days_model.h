#ifndef DAYS_MODEL_H
#define DAYS_MODEL_H

#include <QAbstractListModel>
#include <QDate>
#include <QDebug>
#include <QThread>

class Days_model: public QAbstractListModel
{
    Q_OBJECT

    static const int count_of_dates = 14;

    QHash<int, QByteArray> m_roles;
//    QVector<QString> m_dates;
    QVector<std::tuple<QString, int, int>> m_stats;

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
};

#endif // DAYS_MODEL_H
