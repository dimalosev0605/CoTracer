#ifndef CONTACTS_MODEL_H
#define CONTACTS_MODEL_H

#include <QAbstractListModel>
#include <QDate>
#include <QDebug>
#include <QThread>

#include <tuple>

class Contacts_model: public QAbstractListModel
{
    Q_OBJECT

    QHash<int, QByteArray> m_roles;
    QVector<std::tuple<QString, QString, bool>> m_contacts;
    QVector<QString> m_previous_links;

private:
    QHash<int, QByteArray> roleNames() const override;

public:
    Contacts_model(const QVector<QString>& previous_links, QObject* parent = nullptr);
    Contacts_model() = default;
    ~Contacts_model();

    enum class RolesNames {
        nickname = Qt::UserRole,
        time = Qt::UserRole + 1,
        is_registered = Qt::UserRole + 2
    };

    virtual int rowCount(const QModelIndex &index = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

public slots:
    void receive_unregistered_contacts(const QVector<std::pair<QString, QString>>& contacts);
    void receive_registered_contacts(const QVector<std::pair<QString, QString>>& contacts);
    void remove_contact(int index);
    void add_contact(const QString& nickname, const QString& time, bool is_reg);
};

#endif // CONTACTS_MODEL_H
