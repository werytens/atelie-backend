#ifndef SERVICES_MANAGER_POST_H
#define SERVICES_MANAGER_POST_H

#include <string>
#include <cpprest/json.h>

struct Service {
    std::string name;
    int price;
};

void create_service(std::vector<Service>& services);

#endif // SERVICES_MANAGER_POST_H
