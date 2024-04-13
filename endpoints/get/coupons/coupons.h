#ifndef COUPONS_MANAGER_GET_H
#define COUPONS_MANAGER_GET_H

#include <string>
#include <cpprest/json.h>

using namespace web;

json::value get_all_coupons();

#endif