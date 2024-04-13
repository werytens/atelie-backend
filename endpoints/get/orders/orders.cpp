#include "orders.h"
#include <iostream>
#include <libpq-fe.h>

using namespace std;
using namespace web;

extern PGconn* conn;

extern const char *conninfo;

json::value get_all_orders() {
    json::value orders = json::value::array();

    conn = PQconnectdb(conninfo);
    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        return orders;
    }

    const char* sql = "SELECT * FROM orders";
    PGresult* res = PQexec(conn, sql);

    if (PQresultStatus(res) == PGRES_TUPLES_OK) {
        int rows = PQntuples(res);
        for (int i = 0; i < rows; ++i) {
            string id_order = PQgetvalue(res, i, 0);
            string id_client = PQgetvalue(res, i, 1);
            string date_order = PQgetvalue(res, i, 2);
            string date_order_completed = PQgetvalue(res, i, 3);
            string amount_order = PQgetvalue(res, i, 4);

            json::value order;
            order["id_order"] = json::value::string(id_order);
            order["id_client"] = json::value::string(id_client);
            order["date_order"] = json::value::string(date_order);
            order["date_order_completed"] = json::value::string(date_order_completed);
            order["amount_order"] = json::value::string(amount_order);

            // Fetch order compositions for the current order
            json::value compositions = json::value::array();
            const char* compositions_sql = "SELECT * FROM order_compositions WHERE ID_order = $1";
            const char* param_values[1] = { id_order.c_str() };
            PGresult* compositions_res = PQexecParams(conn, compositions_sql, 1, NULL, param_values, NULL, NULL, 0);

            if (PQresultStatus(compositions_res) == PGRES_TUPLES_OK) {
                int compositions_rows = PQntuples(compositions_res);
                for (int j = 0; j < compositions_rows; ++j) {
                    json::value composition;
                    composition["ID_service"] = json::value::string(PQgetvalue(compositions_res, j, 1));
                    composition["ID_coupon"] = json::value::string(PQgetvalue(compositions_res, j, 2));
                    composition["amount_compositions"] = json::value::string(PQgetvalue(compositions_res, j, 3));
                    compositions[j] = composition;
                }
            }

            order["order_compositions"] = compositions;

            PQclear(compositions_res);
            orders[i] = order;
        }
    }

    PQclear(res);
    PQfinish(conn);

    return orders;
}
