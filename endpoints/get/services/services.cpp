#include "services.h"
#include <iostream>
#include <libpq-fe.h>

using namespace std;
using namespace web;

extern PGconn* conn;

extern const char *conninfo;

json::value get_all_services() {
    json::value services = json::value::array();

    conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        return services;
    }

    const char* sql = "SELECT * FROM services";
    PGresult* res = PQexec(conn, sql);

    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; ++i) {
            string id = PQgetvalue(res, i, 0);
            string name = PQgetvalue(res, i, 1);
            string price = PQgetvalue(res, i, 2);

            json::value service;
            service["id"] = json::value::string(id);
            service["name"] = json::value::string(name);
            service["price"] = json::value::string(price);

            services[i] = service;
        }
    }

    PQclear(res);
    PQfinish(conn);

    return services;
}
