#include "clients.h"
#include <iostream>
#include <libpq-fe.h>

using namespace std;
using namespace web;

extern PGconn* conn;

extern const char *conninfo;

json::value get_all_clients() {
    json::value clients = json::value::array();

    conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        return clients;
    }

    const char* sql = "SELECT * FROM clients";
    PGresult* res = PQexec(conn, sql);

    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; ++i) {
            string id = PQgetvalue(res, i, 0);
            string name = PQgetvalue(res, i, 1);
            string phone = PQgetvalue(res, i, 2);
            string address = PQgetvalue(res, i, 3);
            string email = PQgetvalue(res, i, 4);

            json::value client;
            client["id"] = json::value::string(id);
            client["name"] = json::value::string(name);
            client["phone"] = json::value::string(phone);
            client["address"] = json::value::string(address);
            client["email"] = json::value::string(email);

            clients[i] = client;
        }
    }

    PQclear(res);
    PQfinish(conn);

    return clients;
}
