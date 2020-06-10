#ifndef AVATAR_SELECTOR_H
#define AVATAR_SELECTOR_H

#include <QObject>
#include <QDebug>
#include <QtAndroidExtras/QAndroidActivityResultReceiver>
#include <QtAndroidExtras/qandroidjnienvironment.h>
#include <QtAndroid>

class Avatar_selector: public QObject, public QAndroidActivityResultReceiver
{
    Q_OBJECT
    QString m_selected_image_path;

public:
    virtual void handleActivityResult(int receiverRequestCode, int resultCode, const QAndroidJniObject & data);

public slots:
    void select_avatar();
    QString get_selected_image_path() const;

signals:
    void image_selected(const QString& path);
};

#endif // AVATAR_SELECTOR_H
