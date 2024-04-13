#include "coupons.h"
#include <iostream>
#include <libpq-fe.h>

using namespace std;
using namespace web;

extern PGconn* conn;

extern const char *conninfo;

json::value get_all_coupons() {
    json::value coupons = json::value::array();

    conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        return coupons;
    }

    const char* sql = "SELECT * FROM coupons";
    PGresult* res = PQexec(conn, sql);

    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; ++i) {
            string id = PQgetvalue(res, i, 0);
            string name = PQgetvalue(res, i, 1);
            string discount = PQgetvalue(res, i, 2);

            json::value coupon;
            coupon["id"] = json::value::string(id);
            coupon["name"] = json::value::string(name);
            coupon["discount"] = json::value::string(discount);

            coupons[i] = coupon;
        }
    }

    PQclear(res);
    PQfinish(conn);

    return coupons;
}
