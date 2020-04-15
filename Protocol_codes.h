#ifndef PROTOCOL_CODES_H
#define PROTOCOL_CODES_H

namespace Protocol_codes {

enum class Request_code: int {
    sign_up,
    sign_in,
    add_registered_user,
    add_unregistered_user,
    get_unregistered_contacts,
    get_registered_contacts,
    remove_unregister_contact,
    remove_register_contact
};

enum class Response_code: int {
    internal_server_error,
    success_sign_up,
    sign_up_failure,
    success_sign_in,
    sign_in_failure,
    success_adding,
    such_user_not_exists,
    unregistered_list,
    registered_list,
    success_unregister_contact_deletion,
    unregister_contact_deletion_failure,
    success_register_contact_deletion,
    register_contact_deletion_failure
};

}

#endif // PROTOCOL_CODES_H
