#include "communication_tcp_client.h"

Communication_tcp_client::Communication_tcp_client(QObject* parent)
    : Base_tcp_client(parent)
{
    m_previous_links.push_back(m_user_validator.get_nickname());
}

Contacts_model* Communication_tcp_client::create_model_based_on_date(const QString& date)
{
    Contacts_model* new_model = new Contacts_model(this);
    m_model_date = date;
    m_model_nickname = m_user_validator.get_nickname();

    connect(this, &Communication_tcp_client::contacts_received, new_model, &Contacts_model::receive_contacts,
            Qt::QueuedConnection);
    connect(this, &Communication_tcp_client::success_contact_deletion, new_model, &Contacts_model::remove_contact,
            Qt::QueuedConnection);
    connect(this, &Communication_tcp_client::success_adding, new_model, &Contacts_model::add_contact,
            Qt::QueuedConnection);

    m_models.push_back(new_model);
    request_contacts(Protocol_codes::Request_code::get_unregistered_contacts);
    return new_model;

}

Contacts_model* Communication_tcp_client::create_model_based_on_nickname(const QString& nickname)
{
    m_model_nickname = nickname;
    Contacts_model* new_model = new Contacts_model(m_previous_links, this);

    connect(this, &Communication_tcp_client::contacts_received, new_model, &Contacts_model::receive_contacts,
            Qt::QueuedConnection);

    m_models.push_back(new_model);
    request_contacts(Protocol_codes::Request_code::get_unregistered_contacts);
    return new_model;
}

void Communication_tcp_client::destroy_model()
{
    if(!m_models.isEmpty()) {
        auto poped_model = m_models.back();
        m_models.pop_back();
        delete poped_model;

        if(!m_previous_links_count.isEmpty()) {
            int must_poped = m_previous_links_count.back();
            m_previous_links_count.pop_back();
            for(int i = 0; i < must_poped; ++i) {
                m_previous_links.pop_back();
            }
        }
    }
}

void Communication_tcp_client::parse_response(size_t bytes_transferred)
{
    std::string data = std::string{boost::asio::buffers_begin(m_session->m_response.data()),
                                boost::asio::buffers_begin(m_session->m_response.data()) + bytes_transferred - Protocol_keys::end_of_message.size()};

    auto j_doc = QJsonDocument::fromJson(data.c_str());
    m_session->m_response.consume(bytes_transferred);

//    qDebug() << "Data: " << QString::fromStdString(data);

    if(!j_doc.isEmpty()) {
        auto j_obj = j_doc.object();
        auto j_map = j_obj.toVariantMap();
        m_session->m_res_code = (Protocol_codes::Response_code)j_map[Protocol_keys::response].toInt();

        if(m_session->m_res_code == Protocol_codes::Response_code::unregistered_list ||
           m_session->m_res_code == Protocol_codes::Response_code::registered_list)
        {
            parse_contacts(m_session->m_res_code, j_map);
        }
    }
}

void Communication_tcp_client::parse_contacts(Protocol_codes::Response_code code, const QMap<QString, QVariant>& j_map)
{
    QJsonArray j_arr_of_contacts;
    if(code == Protocol_codes::Response_code::unregistered_list) {
        j_arr_of_contacts = j_map[Protocol_keys::unregistered_list].toJsonArray();
    } else {
        j_arr_of_contacts = j_map[Protocol_keys::registered_list].toJsonArray();
    }

    for(int i = 0; i < j_arr_of_contacts.size(); ++i) {
        auto contact_j_obj = j_arr_of_contacts[i].toObject();
        auto contact_j_obj_map = contact_j_obj.toVariantMap();

        auto pair = std::make_pair(contact_j_obj_map[Protocol_keys::nickname].toString(),
                                   contact_j_obj_map[Protocol_keys::contact_time].toString());

        if(Protocol_codes::Response_code::unregistered_list == code) {
            m_received_contacts.push_back(std::make_tuple(pair.first, pair.second, false));
        }
        if(Protocol_codes::Response_code::registered_list == code) {
            m_received_contacts.push_back(std::make_tuple(pair.first, pair.second, true));
            m_previous_links.push_back(pair.first);
        }
    }
    if(Protocol_codes::Response_code::registered_list == code) {
        m_previous_links_count.push_back(j_arr_of_contacts.size());
    }
}

void Communication_tcp_client::process_data()
{
    switch (m_session->m_res_code) {

    case Protocol_codes::Response_code::success_adding: {
        emit success_adding(m_add_contact_nickname, m_add_contact_time, m_add_contact_is_reg);
        break;
    }
    case Protocol_codes::Response_code::such_user_not_exists: {
        emit info("Such user not exists.");
        break;
    }
    case Protocol_codes::Response_code::internal_server_error: {
        emit info("Internal server error occured. Try later.");
        break;
    }
    case Protocol_codes::Response_code::unregistered_list: {
        request_contacts(Protocol_codes::Request_code::get_registered_contacts);
        break;
    }
    case Protocol_codes::Response_code::registered_list: {
        emit contacts_received(m_received_contacts);
        m_received_contacts.clear();
        disconnect(this, &Communication_tcp_client::contacts_received, nullptr, nullptr);
        break;
    }
    case Protocol_codes::Response_code::success_register_contact_deletion: {
        emit success_contact_deletion(m_index_for_deletion);
        break;
    }
    case Protocol_codes::Response_code::success_unregister_contact_deletion: {
        emit success_contact_deletion(m_index_for_deletion);
        break;
    }
    }

    release();
}

bool Communication_tcp_client::add_contact(int code, const QString& nickname, const QString& time)
{
    if(nickname == m_user_validator.get_nickname()) return false;
    if(!get_is_connected()) {
        emit info("Connection error.");
        return false;
    }
    if(occupy()) {
        m_session->m_request = create_add_contact_req((Protocol_codes::Request_code)code, nickname, time);

        m_add_contact_nickname = nickname;
        m_add_contact_time = time;
        if(Protocol_codes::Request_code::add_registered_user == (Protocol_codes::Request_code)code) {
            m_add_contact_is_reg = true;
        } else {
            m_add_contact_is_reg = false;
        }

        boost::asio::async_write(m_session->m_socket, boost::asio::buffer(m_session->m_request),
                                 boost::bind
                                 (&Communication_tcp_client::on_request_sent,
                                  boost::ref(*this),
                                  boost::placeholders::_1,
                                  boost::placeholders::_2)
                                 );
        return true;
    } else {
        return false;
    }
}

const char* Communication_tcp_client::create_add_contact_req(Protocol_codes::Request_code code, const QString& contact_nickname, const QString& time)
{
    QJsonObject j_obj;

    j_obj.insert(Protocol_keys::request, (int)code);
    j_obj.insert(Protocol_keys::nickname, m_user_validator.get_nickname());
    j_obj.insert(Protocol_keys::contact, contact_nickname);
    j_obj.insert(Protocol_keys::contact_date, m_model_date);
    j_obj.insert(Protocol_keys::contact_time, time);

    QJsonDocument j_doc(j_obj);
    return j_doc.toJson().append(Protocol_keys::end_of_message).data();
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
        set_is_connected(false);
        release();
        qWarning() << "undefined_error()";
        emit info("Error occured.");
    }
}

void Communication_tcp_client::on_response_received(const boost::system::error_code& ec, size_t bytes_transferred)
{
    if(ec.value() == 0) {
        parse_response(bytes_transferred);
        process_data();
    } else {
        set_is_connected(false);
        release();
        qWarning() << "undefined_error()";
        emit info("Error occured.");
    }
}

void Communication_tcp_client::request_contacts(Protocol_codes::Request_code code)
{
    m_session->m_request = create_req_for_contacts(code);
    boost::asio::async_write(m_session->m_socket, boost::asio::buffer(m_session->m_request),
                             boost::bind
                             (&Communication_tcp_client::on_request_sent,
                              boost::ref(*this),
                              boost::placeholders::_1,
                              boost::placeholders::_2)
                             );
}

const char* Communication_tcp_client::create_req_for_contacts(Protocol_codes::Request_code code)
{
    QJsonObject j_obj;

    j_obj.insert(Protocol_keys::request, (int)code);
    j_obj.insert(Protocol_keys::nickname, m_model_nickname);
    j_obj.insert(Protocol_keys::contact_date, m_model_date);

    QJsonDocument j_doc(j_obj);
    return j_doc.toJson().append(Protocol_keys::end_of_message).data();
}

bool Communication_tcp_client::remove_contact(int code, const QString& nickname, const QString& time, int index)
{
    if(!get_is_connected()) {
        emit info("Connection error.");
        return false;
    }
    if(occupy()) {
        m_session->m_request = create_remove_contact_req((Protocol_codes::Request_code)code, nickname, time);
        m_index_for_deletion = index;

        boost::asio::async_write(m_session->m_socket, boost::asio::buffer(m_session->m_request),
                                 boost::bind
                                 (&Communication_tcp_client::on_request_sent,
                                  boost::ref(*this),
                                  boost::placeholders::_1,
                                  boost::placeholders::_2)
                                 );
        return true;
    } else {
        return false;
    }
}

const char* Communication_tcp_client::create_remove_contact_req(Protocol_codes::Request_code code, const QString& nickname, const QString& time)
{
    QJsonObject j_obj;

    j_obj.insert(Protocol_keys::request, (int)code);
    j_obj.insert(Protocol_keys::nickname, m_user_validator.get_nickname());
    j_obj.insert(Protocol_keys::contact_date, m_model_date);
    j_obj.insert(Protocol_keys::contact, nickname);
    j_obj.insert(Protocol_keys::contact_time, time);

    QJsonDocument j_doc(j_obj);
    return j_doc.toJson().append(Protocol_keys::end_of_message).data();
}
