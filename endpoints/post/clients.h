#ifndef CLIENT_MANAGER_POST_H
#define CLIENT_MANAGER_POST_H

#include <string>
#include <cpprest/json.h>

void create_client(const std::string& client_name, const std::string& client_phone, const std::string& client_address, const std::string& client_email);

#endif // CLIENT_MANAGER_H