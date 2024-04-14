#ifndef CLIENT_MANAGER_POST_H
#define CLIENT_MANAGER_POST_H

#include <string>
#include <vector>
#include <cpprest/json.h>

struct Client {
    std::string name;
    std::string phone;
    std::string address;
    std::string email;
};

void create_clients(std::vector<Client>& clients);

#endif
