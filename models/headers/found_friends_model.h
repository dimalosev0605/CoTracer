#ifndef FOUND_FRIENDS_MODEL_H
#define FOUND_FRIENDS_MODEL_H

#include <QAbstractListModel>
#include <QDebug>
#include <QFile>

#include "./miscellaneous/headers/path_finder.h"

class Found_friends_model: public QAbstractListModel
{
    Q_OBJECT

    QHash<int, QByteArray> m_roles;

    QVector <QString> m_found_friends;
    Path_finder m_path_finder;

private:
    QHash<int, QByteArray> roleNames() const override;

public:
    Found_friends_model(QObject* parent = nullptr);

    enum class RolesNames {
        nickname = Qt::UserRole,
        avatar_path = Qt::UserRole + 1
    };

    virtual int rowCount(const QModelIndex &index = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

public slots:
    void receive_found_friends(const QVector<QString>& found_friends);
};

#endif // FOUND_FRIENDS_MODEL_H
