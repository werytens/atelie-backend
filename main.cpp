#include <iostream>
#include <string>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <libpq-fe.h>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

PGconn* conn = nullptr;

const char *conninfo = "dbname=atelie user=postgres password=1234 hostaddr=127.0.0.1 port=5432";

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

void create_client(const string& client_name, const string& client_phone, const string& client_address, const string& client_email) {
    conn = PQconnectdb(conninfo);

    if (PQstatus(conn) != CONNECTION_OK) {
        cerr << "Connection to database failed: " << PQerrorMessage(conn) << endl;
        PQfinish(conn);
        exit(1);
    }

    const char* sql = "INSERT INTO clients (FCS_client, phone_client, address_client, email_client) VALUES ($1, $2, $3, $4)";
    const char* params[4] = {client_name.c_str(), client_phone.c_str(), client_address.c_str(), client_email.c_str()};
    PGresult* res = PQexecParams(conn, sql, 4, nullptr, params, nullptr, nullptr, 0);

    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        cerr << "Failed to insert client: " << PQerrorMessage(conn) << endl;
        PQclear(res);
        PQfinish(conn);
        exit(1);
    }

    PQclear(res);
    PQfinish(conn);
}

int main() {
    http_listener listener("http://localhost:8080");

    listener.support(methods::POST, [](const http_request& request) {
        auto relative_path = uri::decode(request.relative_uri().path());
        if (relative_path == "/clients") {
            request.extract_json().then([=](json::value request_body) {
                string client_name = request_body["client_name"].as_string();
                string client_phone = request_body["client_phone"].as_string();
                string client_address = request_body["client_address"].as_string();
                string client_email = request_body["client_email"].as_string();

                create_client(client_name, client_phone, client_address, client_email);

                request.reply(status_codes::OK);
            }).wait();
        } else {
            request.reply(status_codes::NotFound);
        }
    });

    listener.support(methods::GET, [](const http_request& request) {
        auto relative_path = uri::decode(request.relative_uri().path());
        if (relative_path == "/clients") {
            json::value response_body = get_all_clients();
            
            request.reply(status_codes::OK, response_body);
        } else {
            request.reply(status_codes::NotFound);
        }
    });

    try {
        listener.open().then([&listener]() {
            cout << "Сервер слушает на http://localhost:8080" << endl;
        }).wait();

        while (true);
    } catch (const std::exception& e) {
        cerr << "Ошибка: " << e.what() << endl;
    }

    return 0;
}
// compile:  g++ main.cpp -o main -lboost_system -lcpprest -lssl -lcrypto
// g++ main.cpp -o main -lboost_system -lcpprest -lssl -lcrypto -lpq
// g++ main.cpp -o main -lboost_system -lcpprest -lssl -lcrypto -I/usr/include -lpq