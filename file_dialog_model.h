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
    QVector<std::tuple<QString, bool>> m_entry_list;
    QStringList m_filters;
    QDir m_dir;

private:
    QHash<int, QByteArray> roleNames() const override;
    void clear();
    void insert();

public:
    enum class RolesNames {
        entry_name = Qt::UserRole,
        is_dir_or_file = Qt::UserRole + 1, // true -> dir
    };

    explicit File_dialog_model(QObject *parent = nullptr);
    virtual int rowCount(const QModelIndex &index = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

public slots:
    bool try_cd(const QString& enrty);
    void return_back();
    bool try_choose_avatar(const QString& entry);
    void refresh();

signals:
    void selected(const QString& path);
};

#endif // FILE_DIALOG_MODEL_H
