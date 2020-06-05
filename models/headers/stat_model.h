#ifndef Stat_MODEL_H
#define Stat_MODEL_H

#include <QAbstractListModel>
#include <QDate>
#include <QDebug>
#include <QThread>
#include <QTimer>

class Stat_model: public QAbstractListModel
{
    Q_OBJECT

    static const int count_of_dates = 14;

    QHash<int, QByteArray> m_roles;
    QVector<std::tuple<QString, int>> m_stat;

private:
    QHash<int, QByteArray> roleNames() const override;

public:
    enum class RolesNames {
        date = Qt::UserRole,
        count_of_contacts = Qt::UserRole + 1,
    };

    Stat_model(QObject* parent = nullptr);
    virtual int rowCount(const QModelIndex &index = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

public slots:
    void receive_stats(const QVector<std::tuple<QString, int>>& statistic);
    void sort_by_date();
    void sort_by_count_of_contacts();
    QVector<std::tuple<QString, int>> get_stat() const;
};

#endif // Stat_MODEL_H
