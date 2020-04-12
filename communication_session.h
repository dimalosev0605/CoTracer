#ifndef COMMUNICATION_SESSION_H
#define COMMUNICATION_SESSION_H

#include <string>
#include "boost/asio.hpp"

#include "Protocol_codes.h"

struct Communication_session
{
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::ip::tcp::endpoint m_ep;
    std::string m_request;
    boost::asio::streambuf m_response;

    Protocol_codes::Request_code m_req_code;
    Protocol_codes::Response_code m_res_code;

    Communication_session(boost::asio::io_service& ios, const std::string& raw_ip, unsigned short port_number)
        : m_socket(ios),
          m_ep(boost::asio::ip::address::from_string(raw_ip), port_number)
    {}
};

#endif // COMMUNICATION_SESSION_H
