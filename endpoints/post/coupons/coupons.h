#ifndef COUPONS_MANAGER_POST_H
#define COUPONS_MANAGER_POST_H

#include <string>
#include <cpprest/json.h>

void create_coupon(const std::string& coupon_name, int coupon_discount);

#endif // COUPONS_MANAGER_H