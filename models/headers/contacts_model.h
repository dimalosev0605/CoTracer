#ifndef CONTACTS_MODEL_H
#define CONTACTS_MODEL_H

#include <QAbstractListModel>
#include <QDate>
#include <QDebug>
#include <QThread>
#include <QCoreApplication>

#include <tuple>

class Contacts_model: public QAbstractListModel
{
    Q_OBJECT

    QHash<int, QByteArray> m_roles;
    QVector<std::tuple<QString, QString>> m_contacts; // nickname, contact time
    QVector<QString> m_previous_links;

private:
    QHash<int, QByteArray> roleNames() const override;

public:
    Contacts_model(QObject* parent = nullptr);
    Contacts_model(const QVector<QString>& previous_links, QObject* parent = nullptr);
    ~Contacts_model();

    enum class RolesNames {
        contact_nickname = Qt::UserRole,
        contact_time = Qt::UserRole + 1,
        contact_avatar_path = Qt::UserRole + 2,
    };

    virtual int rowCount(const QModelIndex &index = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

public slots:
    void remove_contact(int index);
    void add_contact(const QString& nickname, const QString& time);
    void receive_contacts(const QVector<std::tuple<QString, QString>>& contacts);
};

#endif // CONTACTS_MODEL_H
