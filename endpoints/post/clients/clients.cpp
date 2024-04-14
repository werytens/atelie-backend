#include "clients.h"
#include <iostream>
#include <libpq-fe.h>

using namespace std;

extern PGconn* conn;

extern const char *conninfo;

void create_clients(std::vector<Client>& clients) { 
    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        exit(1);
    }

    const char* sql = "INSERT INTO clients (FCS_client, phone_client, address_client, email_client) VALUES ($1, $2, $3, $4)";
    for (const auto& client : clients) {
        const char* params[4] = {client.name.c_str(), client.phone.c_str(), client.address.c_str(), client.email.c_str()};
        PGresult* res = PQexecParams(conn, sql, 4, nullptr, params, nullptr, nullptr, 0);

        if (PQresultStatus(res) != PGRES_COMMAND_OK) {
            cerr << "Failed to insert client: " << PQerrorMessage(conn) << endl;
            PQclear(res);
            PQfinish(conn);
            exit(1);
        }

        PQclear(res);
    }

    PQfinish(conn);
}
