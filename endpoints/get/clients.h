#ifndef CLIENT_MANAGER_GET_H
#define CLIENT_MANAGER_GET_H

#include <string>
#include <cpprest/json.h>

using namespace web;

json::value get_all_clients();

#endif // CLIENT_MANAGER_H