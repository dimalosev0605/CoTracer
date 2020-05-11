#ifndef FILE_DIALOG_MODEL_H
#define FILE_DIALOG_MODEL_H

#include <QAbstractListModel>
#include <QFile>
#include <QDir>
#include <QDebug>

class File_dialog_model : public QAbstractListModel
{
    Q_OBJECT

    QHash<int, QByteArray> m_roles;

    QString m_curr_abs_path;
    QVector<QString> m_entry_list;

private:
    QHash<int, QByteArray> roleNames() const override;

public:
    enum class RolesNames {
        entry_name = Qt::UserRole,
    };

    explicit File_dialog_model(QObject *parent = nullptr);
    virtual int rowCount(const QModelIndex &index = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

public slots:
    bool try_cd(const QString& enrty);
    void return_back();
    bool try_choose_photo(const QString& entry);

signals:
    void photo(const QString& path);
};

#endif // FILE_DIALOG_MODEL_H
