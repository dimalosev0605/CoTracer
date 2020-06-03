#ifndef FILE_DIALOG_MODEL_H
#define FILE_DIALOG_MODEL_H

#include <QAbstractListModel>
#include <QFile>
#include <QDir>
#include <QDateTime>
#include <QDebug>

class File_dialog_model : public QAbstractListModel
{
    Q_OBJECT

    QHash<int, QByteArray> m_roles;

    QString m_curr_abs_path;
    QVector<std::tuple<QString, QString, QString>> m_data; // name, last modified time, path_to_img
    QVector<std::tuple<QString, QString, QString>> m_data_copy;
    QDir m_dir;

private:
    QHash<int, QByteArray> roleNames() const override;
    void clear();
    void update_content(QDir::SortFlag sort_flag = QDir::NoSort);
    void set_curr_abs_path(const QString& path);

public:
    enum class RolesNames {
        name = Qt::UserRole,
        last_modified_time = Qt::UserRole + 1,
        path_to_img = Qt::UserRole + 2,
    };

    explicit File_dialog_model(QObject *parent = nullptr);
    virtual int rowCount(const QModelIndex &index = QModelIndex()) const override;
    virtual QVariant data(const QModelIndex& index, int role) const override;

public slots:
    bool cd(const QString& directory);
    void return_in_parent_directory();
    bool choose_avatar(const QString& entry);
    void refresh();
    QString get_current_path() const;

    void search(const QString& input);
    void prepare_search();
    void cancel_search();
    void sort_by_time();
    void sort_by_name();

signals:
    void selected(const QString& path);
    void current_path_changed(const QString& path);
};

#endif // FILE_DIALOG_MODEL_H
