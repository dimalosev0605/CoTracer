#ifndef SETTINGS_MODEL_H
#define SETTINGS_MODEL_H

#include <QAbstractListModel>

class Settings_model: public QAbstractListModel
{
    Q_OBJECT
    QHash<int, QByteArray> m_roles;
    QVector<std::tuple<QString, int>> m_settings;

private:
    QHash<int, QByteArray> roleNames() const override;

public:
    Settings_model(QObject* parent = nullptr);

    enum class RolesNames {
        settings_name = Qt::UserRole,
        settings_number = Qt::UserRole + 1
    };

    virtual int rowCount(const QModelIndex &index = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;
};

#endif // SETTINGS_MODEL_H
