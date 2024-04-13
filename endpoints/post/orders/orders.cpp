#include "orders.h"
#include <iostream>
#include <libpq-fe.h>

using namespace std;

extern PGconn* conn;
extern const char *conninfo;

void create_order(int order_client_id, const std::string& order_date, int order_amount) {
    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        exit(1);
    }

    const char* sql = "INSERT INTO orders (ID_client, date_order, amount_order) VALUES ($1, $2, $3)";
    const char* params[3] = {to_string(order_client_id).c_str(), order_date.c_str(), to_string(order_amount).c_str()};

    PGresult* res = PQexecParams(conn, sql, 3, nullptr, params, nullptr, nullptr, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Failed to insert service: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }

    PQclear(res);
    PQfinish(conn);
}

void insert_service(int order_id, int service_id, int* coupon_id, int service_amount) {
    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        exit(1);
    }

    PGresult* res;
   
    if (coupon_id == nullptr) {
        const char* sql = "INSERT INTO orders (ID_order, ID_service, amount_compositions) VALUES ($1, $2, $3)";
        const char* otherParams[3] = {to_string(order_id).c_str(), to_string(service_id).c_str(), to_string(service_amount).c_str()};
        res = PQexecParams(conn, sql, 3, nullptr, otherParams, nullptr, nullptr, 0);
    } else {
        const char* sql = "INSERT INTO orders (ID_order, ID_service, ID_coupon, amount_compositions) VALUES ($1, $2, $3, $4)";
        const char* params[4] = {to_string(order_id).c_str(), to_string(service_id).c_str(), to_string(*coupon_id).c_str(), to_string(service_amount).c_str()};
        res = PQexecParams(conn, sql, 4, nullptr, params, nullptr, nullptr, 0);
    } 

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Failed to insert service: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }

    PQclear(res);
    PQfinish(conn);
}

void complete_order(int order_id) {
    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        exit(1);
    }

    const char* sql = "UPDATE orders set date_order_completed = CURRENT_DATE where ID_order = $1;";
    const char* params[1] = {to_string(order_id).c_str()};

    PGresult* res = PQexecParams(conn, sql, 1, nullptr, params, nullptr, nullptr, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Failed to insert service: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }

    PQclear(res);
    PQfinish(conn);
}