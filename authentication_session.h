#ifndef AUTHENTICATION_SESSION_H
#define AUTHENTICATION_SESSION_H

#include <string>
#include "boost/asio.hpp"

struct Authentication_session
{
    enum class Request_code: int {
        sign_up,
        sign_in
    };

    enum class Response_code: int {
        internal_server_error,
        success_sign_up,
        sign_up_failure,
        success_sign_in,
        sign_in_failure
    };

    boost::asio::ip::tcp::socket m_socket;
    boost::asio::ip::tcp::endpoint m_ep;
    std::string m_request;
    boost::asio::streambuf m_response;

    Request_code m_req_code;
    Response_code m_res_code;

    Authentication_session(boost::asio::io_service& ios, const std::string& raw_ip, unsigned short port_number)
        : m_socket(ios),
          m_ep(boost::asio::ip::address::from_string(raw_ip), port_number)
    {}
};

#endif // AUTHENTICATION_SESSION_H
