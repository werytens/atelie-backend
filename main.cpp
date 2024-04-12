#include <iostream>
#include <string>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <libpq-fe.h>
#include "endpoints/post/clients.h"
#include "endpoints/get/clients.h" // Обязательно включите заголовочный файл здесь

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

PGconn* conn = nullptr;

const char *conninfo = "dbname=atelie user=postgres password=1234 hostaddr=127.0.0.1 port=5432";

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
