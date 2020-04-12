#ifndef DAYS_MODEL_H
#define DAYS_MODEL_H

#include <QAbstractListModel>
#include <QDate>
#include <QDebug>

#include "contacts_model.h"
#include "communication_tcp_client.h"

class Days_model: public QAbstractListModel
{
    Q_OBJECT

    static const int count_of_dates = 14;

    QHash<int, QByteArray> m_roles;
    QVector<QString> m_dates;
    QList<Contacts_model*> m_models;
    Communication_tcp_client m_client;

private:
    QHash<int, QByteArray> roleNames() const override;
    void fill_m_dates();

public:
    enum class RolesNames {
        date = Qt::UserRole,
    };

    Days_model(QObject* parent = nullptr);
    virtual int rowCount(const QModelIndex &index = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

public slots:
    Contacts_model* create_model();
    void destroy_model();
};

#endif // DAYS_MODEL_H
