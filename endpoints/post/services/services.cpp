#include "services.h"
#include <iostream>
#include <libpq-fe.h>

using namespace std;

extern PGconn* conn;
extern const char *conninfo;

void create_service(std::vector<Service>& services) {
    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        exit(1);
    }

    const char* sql = "INSERT INTO services (name_service, price_service) VALUES ($1, $2)";
    
    for (const auto& service : services) {
        const char* params[2] = {service.name.c_str(), to_string(service.price).c_str()};

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
