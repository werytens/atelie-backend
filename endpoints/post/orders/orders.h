#ifndef ORDERS_MANAGER_POST_H
#define ORDERS_MANAGER_POST_H

#include <string>
#include <cpprest/json.h>

void create_order(int order_client_id, const std::string& order_date, int order_amount);
void insert_service(int order_id, int service_id, int coupon_id, int std::string& service_amount);
void complete_order(int order_id);

#endif // ORDERS_MANAGER_H