#include <QApplication>
#include <QQmlApplicationEngine>

//#include "./networking/headers/authorization_tcp_client.h"
#include "./networking/headers/communication_tcp_client.h"
#include "./models/headers/days_model.h"
#include "./models/headers/contacts_model.h"
#include "./models/headers/chart_data.h"
#include "./models/headers/file_dialog_model.h"
#include "./models/headers/settings_model.h"
#include "./networking/headers/client.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

//    qmlRegisterType<Authorization_tcp_client>("Authorization_tcp_client_qml", 1, 0, "Authorization_tcp_client");
    qmlRegisterType<Communication_tcp_client>("Communication_tcp_client_qml", 1, 0, "Communication_tcp_client");
    qmlRegisterType<Days_model>("Days_model_qml", 1, 0, "Days_model");
    qmlRegisterType<Contacts_model>("Contacts_model_qml", 1, 0, "Contacts_model");
    qmlRegisterType<Chart_data>("Chart_data_qml", 1, 0, "Chart_data");
    qmlRegisterType<File_dialog_model>("File_dialog_model_qml", 1, 0, "File_dialog_model");
    qmlRegisterType<Settings_model>("Settings_model_qml", 1, 0, "Settings_model");
    qmlRegisterType<Client>("Client_qml", 1, 0, "Client");

    qRegisterMetaType<QVector<std::tuple<QString, QString, bool>>>("QVector<std::tuple<QString, QString, bool>>");
    qRegisterMetaType<QVector<std::tuple<QString, int, int>>>("QVector<std::tuple<QString, int, int>>");
    qRegisterMetaType<QVector<std::tuple<int, int>>>("QVector<std::tuple<int, int>>");

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
