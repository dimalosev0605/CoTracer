#ifndef PROTOCOL_CODES_H
#define PROTOCOL_CODES_H

namespace Protocol_codes {

enum class Request_code: int {
    sign_up,
    sign_in,
    add_contact,
    remove_contact,
    fetch_stat_for_14_days,
    fetch_contacts,
    change_avatar,
    change_password
};

enum class Response_code: int {
    internal_server_error,
    success_sign_up,
    sign_up_failure,
    success_sign_in,
    sign_in_failure,
    success_contact_adding,
    such_contact_not_exists,
    success_contact_deletion,
    success_fetching_stat_for_14_days,
    success_fetching_contacts,
    success_avatar_changing,
    success_password_changing
};

}

#endif // PROTOCOL_CODES_H
