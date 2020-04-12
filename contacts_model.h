#ifndef CONTACTS_MODEL_H
#define CONTACTS_MODEL_H

#include <QAbstractListModel>
#include <QDate>
#include <QDebug>

class Contacts_model: public QAbstractListModel
{
    Q_OBJECT

    QHash<int, QByteArray> m_roles;
    QVector<QString> m_contacts;

private:
    QHash<int, QByteArray> roleNames() const override;

public:
    Contacts_model(QObject* parent = nullptr);
    ~Contacts_model();

    enum class RolesNames {
        date = Qt::UserRole,
    };

    virtual int rowCount(const QModelIndex &index = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
};

#endif // CONTACTS_MODEL_H
