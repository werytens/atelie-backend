#include "services.h"
#include <iostream>
#include <libpq-fe.h>

using namespace std;

extern PGconn* conn;
extern const char *conninfo;

void create_coupon(const std::string& coupon_name, int coupon_discount) {
    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        exit(1);
    }

    const char* sql = "INSERT INTO coupons (coupon_name, coupon_discount) VALUES ($1, $2)";
    const char* params[2] = {coupon_name.c_str(), to_string(coupon_discount).c_str()};

    PGresult* res = PQexecParams(conn, sql, 2, nullptr, params, nullptr, nullptr, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Failed to insert service: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }

    PQclear(res);
    PQfinish(conn);
}
