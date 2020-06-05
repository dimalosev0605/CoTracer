#include <QApplication>
#include <QQmlApplicationEngine>

//#include "./networking/headers/authorization_tcp_client.h"
//#include "./networking/headers/communication_tcp_client.h"
#include "./models/headers/stat_model.h"
#include "./models/headers/contacts_model.h"
#include "./models/headers/chart_data.h"
#include "./models/headers/file_dialog_model.h"
#include "./models/headers/settings_model.h"
#include "./networking/headers/client.h"

int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);

    QApplication app(argc, argv);

    qmlRegisterType<Stat_model>("Stat_model_qml", 1, 0, "Stat_model");
    qmlRegisterType<Contacts_model>("Contacts_model_qml", 1, 0, "Contacts_model");
    qmlRegisterType<Chart_data>("Chart_data_qml", 1, 0, "Chart_data");
    qmlRegisterType<File_dialog_model>("File_dialog_model_qml", 1, 0, "File_dialog_model");
    qmlRegisterType<Settings_model>("Settings_model_qml", 1, 0, "Settings_model");
    qmlRegisterType<Client>("Client_qml", 1, 0, "Client");

    qRegisterMetaType<QVector<std::tuple<QString, QString>>>("QVector<std::tuple<QString, QString>>");
    qRegisterMetaType<QVector<std::tuple<QString, int>>>("QVector<std::tuple<QString, int>>");

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
