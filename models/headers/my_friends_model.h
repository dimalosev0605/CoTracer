#ifndef MY_FRIENDS_MODEL_H
#define MY_FRIENDS_MODEL_H

#include <QAbstractListModel>
#include <QDebug>

class My_friends_model: public QAbstractListModel
{
    Q_OBJECT

    QHash<int, QByteArray> m_roles;

private:
    QHash<int, QByteArray> roleNames() const override;

public:
    My_friends_model(QObject* parent = nullptr);

    enum class RolesNames {

    };

    virtual int rowCount(const QModelIndex &index = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

public slots:

};

#endif // MY_FRIENDS_MODEL_H
