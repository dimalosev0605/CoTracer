#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "authentication_tcp_client.h"
#include "days_model.h"
#include "contacts_model.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QGuiApplication app(argc, argv);

    qmlRegisterType<Authentication_tcp_client>("Authentication_tcp_client_qml", 1, 0, "Authentication_tcp_client");
    qmlRegisterType<Days_model>("Days_model_qml", 1, 0, "Days_model");
    qmlRegisterType<Contacts_model>("Contacts_model_qml", 1, 0, "Contacts_model");

    QQmlApplicationEngine engine;
    const QUrl url(QStringLiteral("qrc:/qml_files/main.qml"));
    QObject::connect(&engine, &QQmlApplicationEngine::objectCreated,
                     &app, [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
            QCoreApplication::exit(-1);
    }, Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}