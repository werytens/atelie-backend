#include "coupons.h"
#include <iostream>
#include <libpq-fe.h>

using namespace std;

extern PGconn* conn;
extern const char *conninfo;

void create_coupon(std::vector<Coupon>& coupons) {
    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        exit(1);
    }

    const char* sql = "INSERT INTO coupons (coupon_name, coupon_discount) VALUES ($1, $2)";

    for (const auto& coupon : coupons) {
        const char* params[2] = {coupon.name.c_str(), to_string(coupon.discount).c_str()};

        PGresult* res = PQexecParams(conn, sql, 2, nullptr, params, nullptr, nullptr, 0);

        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            cerr << "Failed to insert service: " << PQerrorMessage(conn) << endl;
            PQclear(res);
            PQfinish(conn);
            exit(1);
        }

        PQclear(res);
    }

    
    PQfinish(conn);
}
