#ifndef MY_FRIENDS_MODEL_H
#define MY_FRIENDS_MODEL_H

#include <QAbstractListModel>
#include <QDebug>
#include <QFile>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "./miscellaneous/headers/path_finder.h"
#include "./networking/headers/Protocol_keys.h"

class My_friends_model: public QAbstractListModel
{
    Q_OBJECT

    QHash<int, QByteArray> m_roles;

    QVector<QString> m_friends;
    QVector<QString> m_sorted_friends;
    Path_finder m_path_finder;

private:
    QHash<int, QByteArray> roleNames() const override;

    void load_friends_list();
    void save_friends_list();
    void sort_friends_list();

public:
    My_friends_model(QObject* parent = nullptr);
    ~My_friends_model();

    enum class RolesNames {
        friend_nickname = Qt::UserRole,
        friend_avatar_path = Qt::UserRole + 1,
        delegate_type = Qt::UserRole + 2
    };

    virtual int rowCount(const QModelIndex &index = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

public slots:
    void add_friend(const QString& new_friend);
    void remove_friend(const QString& nickname);
};

#endif // MY_FRIENDS_MODEL_H
