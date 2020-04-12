#ifndef COMMUNICATION_TCP_CLIENT_H
#define COMMUNICATION_TCP_CLIENT_H


#include <QObject>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>

#include <boost/bind/bind.hpp>

#include "user_validator.h"
#include "communication_session.h"

class Communication_tcp_client: public QObject
{
    Q_OBJECT

    boost::asio::io_service m_ios;
    std::unique_ptr<boost::asio::io_service::work> m_work;
    std::unique_ptr<std::thread> m_thread;
    std::unique_ptr<Communication_session> m_session;
    User_validator m_user_validator;

private:
    void connect_to_server();

public:
    explicit Communication_tcp_client(QObject* parent = nullptr);
    ~Communication_tcp_client();

};

#endif // COMMUNICATION_TCP_CLIENT_H
