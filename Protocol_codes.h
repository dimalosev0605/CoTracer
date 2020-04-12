#ifndef PROTOCOL_CODES_H
#define PROTOCOL_CODES_H

namespace Protocol_codes {

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

}

#endif // PROTOCOL_CODES_H
