#ifndef PROTOCOL_CODES_H
#define PROTOCOL_CODES_H

namespace Protocol_codes {

enum class Request_code: int {
    sign_up,
    sign_in,
    add_registered_user,
    add_unregistered_user,
    remove_unregister_contact,
    remove_register_contact,
    stats_for_14_days,
    get_contacts,
    change_avatar
};

enum class Response_code: int {
    internal_server_error,
    success_sign_up,
    sign_up_failure,
    success_sign_in,
    sign_in_failure,
    success_adding,
    such_user_not_exists,
    success_unregister_contact_deletion,
    success_register_contact_deletion,
    success_fetch_stats_for_14_days,
    contacts_list,
    success_avatar_changing
};

}

#endif // PROTOCOL_CODES_H
