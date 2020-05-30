#ifndef SESSION_H
#define SESSION_H

#include <string>

#include "boost/asio.hpp"

struct Session
{
    boost::asio::ip::tcp::socket m_socket;
    boost::asio::ip::tcp::endpoint m_ep;

    std::string m_request;
    boost::asio::streambuf m_response;

    Session(boost::asio::io_service& ios, const std::string& raw_ip, unsigned short port_number)
        : m_socket(ios),
          m_ep(boost::asio::ip::address::from_string(raw_ip), port_number)
    {}
};

#endif // SESSION_H
