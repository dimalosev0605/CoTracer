#ifndef PROTOCOL_KEYS_H
#define PROTOCOL_KEYS_H

#include <QString>

namespace Protocol_keys {

const QString user_nickname = "user_nickname";
const QString user_password = "user_password";

const QString request_code = "request_code";
const QString response_code = "response_code";

const QString contact_nickname = "contact_nickname";
const QString contact_time = "contact_time";
const QString contact_date = "contact_date";
const QString contact_list = "contact_list";

const QString statistic_for_14_days = "statistic_for_14_days";
const QString quantity_of_contacts = "quantity_of_contacts";
const QString stat_date = "stat_date";

const QString avatar_data = "avatar_data";
const QString avatar_list = "avatar_list";

const QString end_of_message = "\r\n\r\n";

}

#endif // PROTOCOL_KEYS_H
