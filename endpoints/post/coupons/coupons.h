#ifndef COUPONS_MANAGER_POST_H
#define COUPONS_MANAGER_POST_H

#include <string>
#include <cpprest/json.h>

struct Coupon {
    std::string name;
    int discount;
};

void create_coupon(std::vector<Coupon>& coupons);

#endif // COUPONS_MANAGER_H