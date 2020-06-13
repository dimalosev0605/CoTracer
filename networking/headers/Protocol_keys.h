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
const QString is_contact_avatar_cached = "is_contact_avatar_cached";

const QString statistic_for_14_days = "statistic_for_14_days";
const QString quantity_of_contacts = "quantity_of_contacts";
const QString stat_date = "stat_date";

const QString cached_avatars = "cached_avatars";
const QString avatar_data = "avatar_data";
const QString avatar_downloaded_date_time = "avatar_downloaded_date_time";
const QString avatar_downloaded_date_time_format = "dd.MM.yyyy-hh:mm:ss";
const QString deleted_avatar = "000";

const QString end_of_message = "\r\n\r\n";

}

#endif // PROTOCOL_KEYS_H
