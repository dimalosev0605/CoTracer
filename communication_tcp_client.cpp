#include "communication_tcp_client.h"

Communication_tcp_client::Communication_tcp_client(QObject* parent)
    : Base_tcp_client(parent)
{

}

Contacts_model* Communication_tcp_client::create_main_model(const QString& date)
{
    m_date = date;
    m_nickname = m_user_validator.get_nickname();

    Contacts_model* p = new Contacts_model;

    connect(this, &Communication_tcp_client::unregistered_list, p, &Contacts_model::receive_unregistered_contacts,
            Qt::QueuedConnection);
    connect(this, &Communication_tcp_client::registered_list, p, &Contacts_model::receive_registered_contacts,
            Qt::QueuedConnection);

    m_contacts_models.push_back(p);

    get_all_contacts_with_unregistered();

    return p;
}

Contacts_model* Communication_tcp_client::create_new_view(const QString& nickname)
{
    m_nickname = nickname;
    Contacts_model* p = new Contacts_model;

    connect(this, &Communication_tcp_client::unregistered_list, p, &Contacts_model::receive_unregistered_contacts,
            Qt::QueuedConnection);
    connect(this, &Communication_tcp_client::registered_list, p, &Contacts_model::receive_registered_contacts,
            Qt::QueuedConnection);

    m_contacts_models.push_back(p);

    get_all_contacts_with_unregistered();

    return p;
}

void Communication_tcp_client::destroy_model()
{
    auto temp = m_contacts_models.back();
    m_contacts_models.pop_back();
    delete temp;
}

const char* Communication_tcp_client::add_reg_user_req(const QString& contact_nickname, const QString& time)
{
    QJsonObject j_obj;

    j_obj.insert(Protocol_keys::request, (int)Protocol_codes::Request_code::add_registered_user);
    j_obj.insert(Protocol_keys::nickname, m_user_validator.get_nickname());
    j_obj.insert(Protocol_keys::contact, contact_nickname);
    j_obj.insert(Protocol_keys::contact_date, m_date);
    j_obj.insert(Protocol_keys::contact_time, time);

    QJsonDocument j_doc(j_obj);
    return j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

const char* Communication_tcp_client::add_unreg_user_req(const QString& contact_nickname, const QString& time)
{
    QJsonObject j_obj;

    j_obj.insert(Protocol_keys::request, (int)Protocol_codes::Request_code::add_unregistered_user);
    j_obj.insert(Protocol_keys::nickname, m_user_validator.get_nickname());
    j_obj.insert(Protocol_keys::contact, contact_nickname);
    j_obj.insert(Protocol_keys::contact_date, m_date);
    j_obj.insert(Protocol_keys::contact_time, time);

    QJsonDocument j_doc(j_obj);
    return j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

void Communication_tcp_client::parse_response(size_t bytes_transferred)
{
//    std::string data = std::string{boost::asio::buffers_begin(m_session->m_response.data()),
//                                boost::asio::buffers_begin(m_session->m_response.data()) + bytes_transferred - 4};
// from https://stackoverflow.com/questions/40561097/read-until-a-string-delimiter-in-boostasiostreambuf

    std::string data((const char*)m_session->m_response.data().data(), bytes_transferred - 4);
    auto j_doc = QJsonDocument::fromJson(data.c_str());
    m_session->m_response.consume(bytes_transferred);

//    qDebug() << "Data: " << j_doc.toJson();

    if(!j_doc.isEmpty()) {
        auto j_obj = j_doc.object();
        auto j_map = j_obj.toVariantMap();
        m_session->m_res_code = (Protocol_codes::Response_code)j_map[Protocol_keys::response].toInt();

        if(m_session->m_res_code == Protocol_codes::Response_code::unregistered_list) {

            auto j_arr_of_unreg_contacts = j_map[Protocol_keys::unregistered_list].toJsonArray();
            for(int i = 0; i < j_arr_of_unreg_contacts.size(); ++i) {
                auto contact_j_obj = j_arr_of_unreg_contacts[i].toObject();
                auto contact_j_obj_map = contact_j_obj.toVariantMap();

                auto pair = std::make_pair(contact_j_obj_map[Protocol_keys::nickname].toString(),
                                           contact_j_obj_map[Protocol_keys::contact_time].toString());
                m_unregistered_contacts.push_back(pair);
            }

//            qDebug() << "Unregistered Contacts in tcp_client:";
//            for(int i = 0; i < m_unregistered_contacts.size(); ++i) {
//                qDebug() << m_unregistered_contacts[i].first << " - " << m_unregistered_contacts[i].second;
//            }

        }

        if(m_session->m_res_code == Protocol_codes::Response_code::registered_list) {

            auto j_arr_of_reg_contacts = j_map[Protocol_keys::registered_list].toJsonArray();
            for(int i = 0; i < j_arr_of_reg_contacts.size(); ++i) {
                auto contact_j_obj = j_arr_of_reg_contacts[i].toObject();
                auto contact_j_obj_map = contact_j_obj.toVariantMap();

                auto pair = std::make_pair(contact_j_obj_map[Protocol_keys::nickname].toString(),
                                           contact_j_obj_map[Protocol_keys::contact_time].toString());
                m_registered_contacts.push_back(pair);
            }

//            qDebug() << "Registered Contacts in tcp_client:";
//            for(int i = 0; i < m_registered_contacts.size(); ++i) {
//                qDebug() << m_registered_contacts[i].first << " - " << m_registered_contacts[i].second;
//            }

        }

    }

}

void Communication_tcp_client::process_data()
{
    switch (m_session->m_res_code) {

    case Protocol_codes::Response_code::success_adding: {
        emit success_adding();
        break;
    }

    case Protocol_codes::Response_code::internal_server_error: {
        emit internal_server_error();
        break;
    }

    case Protocol_codes::Response_code::unregistered_list: {
        emit unregistered_list(m_unregistered_contacts);
        m_unregistered_contacts.clear();
        get_all_contacts_with_registered();
        break;
    }
    case Protocol_codes::Response_code::registered_list: {
        emit registered_list(m_registered_contacts);
        m_registered_contacts.clear();
        disconnect(this, nullptr, m_contacts_models.last(), nullptr);
        break;
    }
//    case Protocol_codes::Response_code::

    }
}

void Communication_tcp_client::add_registered_contact(const QString& nickaname, const QString& time)
{
    m_session->m_request = add_reg_user_req(nickaname, time);

    boost::asio::async_write(m_session->m_socket, boost::asio::buffer(m_session->m_request),
                             boost::bind
                             (&Communication_tcp_client::on_request_sent,
                              boost::ref(*this),
                              boost::placeholders::_1,
                              boost::placeholders::_2)
                             );
}

void Communication_tcp_client::add_unregistered_contact(const QString& nickname, const QString& time)
{
    m_session->m_request = add_unreg_user_req(nickname, time);

    boost::asio::async_write(m_session->m_socket, boost::asio::buffer(m_session->m_request),
                             boost::bind
                             (&Communication_tcp_client::on_request_sent,
                              boost::ref(*this),
                              boost::placeholders::_1,
                              boost::placeholders::_2)
                             );
}

void Communication_tcp_client::on_request_sent(const boost::system::error_code& ec, size_t bytes_transferred)
{
    m_session->m_request.clear();
    if(ec.value() == 0) {
        boost::asio::async_read_until(m_session->m_socket, m_session->m_response, Protocol_keys::end_of_message.toStdString(),
                                      boost::bind
                                      (&Communication_tcp_client::on_response_received,
                                       boost::ref(*this),
                                       boost::placeholders::_1,
                                       boost::placeholders::_2)
                                      );
    } else {
    }
}

void Communication_tcp_client::on_response_received(const boost::system::error_code& ec, size_t bytes_transferred)
{
    if(ec.value() == 0) {
        parse_response(bytes_transferred);
        process_data();
    } else {

    }
}

void Communication_tcp_client::get_all_contacts_with_registered()
{
    m_session->m_request = get_all_contacts_with_registered_req();

    boost::asio::async_write(m_session->m_socket, boost::asio::buffer(m_session->m_request),
                             boost::bind
                             (&Communication_tcp_client::on_request_sent,
                              boost::ref(*this),
                              boost::placeholders::_1,
                              boost::placeholders::_2)
                             );
}

void Communication_tcp_client::get_all_contacts_with_unregistered()
{
    m_session->m_request = get_all_contacts_with_unregistered_req();

    boost::asio::async_write(m_session->m_socket, boost::asio::buffer(m_session->m_request),
                             boost::bind
                             (&Communication_tcp_client::on_request_sent,
                              boost::ref(*this),
                              boost::placeholders::_1,
                              boost::placeholders::_2)
                             );
}

const char* Communication_tcp_client::get_all_contacts_with_registered_req()
{
    QJsonObject j_obj;

    j_obj.insert(Protocol_keys::request, (int)Protocol_codes::Request_code::get_registered_contacts);
    j_obj.insert(Protocol_keys::nickname, m_nickname);
    j_obj.insert(Protocol_keys::contact_date, m_date);

    QJsonDocument j_doc(j_obj);
    return j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

const char* Communication_tcp_client::get_all_contacts_with_unregistered_req()
{
    QJsonObject j_obj;

    j_obj.insert(Protocol_keys::request, (int)Protocol_codes::Request_code::get_unregistered_contacts);
    j_obj.insert(Protocol_keys::nickname, m_nickname);
    j_obj.insert(Protocol_keys::contact_date, m_date);

    QJsonDocument j_doc(j_obj);
    return j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

void Communication_tcp_client::remove_unregistered_contact(const QString& nickname, const QString& time)
{
    qDebug() << "Removing UNregistered contact: " << nickname + " - " + time;
    m_session->m_request = remove_unregistered_contact_req(nickname, time);

    boost::asio::async_write(m_session->m_socket, boost::asio::buffer(m_session->m_request),
                             boost::bind
                             (&Communication_tcp_client::on_request_sent,
                              boost::ref(*this),
                              boost::placeholders::_1,
                              boost::placeholders::_2)
                             );
}

void Communication_tcp_client::remove_registered_contact(const QString& nickname, const QString& time)
{
    qDebug() << "Removing Registered contact: " << nickname + " - " + time;
    m_session->m_request = remove_registered_contact_req(nickname, time);

    boost::asio::async_write(m_session->m_socket, boost::asio::buffer(m_session->m_request),
                             boost::bind
                             (&Communication_tcp_client::on_request_sent,
                              boost::ref(*this),
                              boost::placeholders::_1,
                              boost::placeholders::_2)
                             );
}

const char* Communication_tcp_client::remove_unregistered_contact_req(const QString& nickname, const QString& time)
{
    QJsonObject j_obj;

    j_obj.insert(Protocol_keys::request, (int)Protocol_codes::Request_code::remove_unregister_contact);
    j_obj.insert(Protocol_keys::nickname, m_user_validator.get_nickname());
    j_obj.insert(Protocol_keys::contact_date, m_date);
    j_obj.insert(Protocol_keys::contact, nickname);
    j_obj.insert(Protocol_keys::contact_time, time);

    QJsonDocument j_doc(j_obj);
    return j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

const char* Communication_tcp_client::remove_registered_contact_req(const QString& nickname, const QString& time)
{
    QJsonObject j_obj;

    j_obj.insert(Protocol_keys::request, (int)Protocol_codes::Request_code::remove_register_contact);
    j_obj.insert(Protocol_keys::nickname, m_user_validator.get_nickname());
    j_obj.insert(Protocol_keys::contact_date, m_date);
    j_obj.insert(Protocol_keys::contact, nickname);
    j_obj.insert(Protocol_keys::contact_time, time);

    QJsonDocument j_doc(j_obj);
    return j_doc.toJson().append(Protocol_keys::end_of_message).data();
}
