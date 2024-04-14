#include <iostream>
#include <string>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <cpprest/uri.h>
#include <postgresql/libpq-fe.h>
#include "endpoints/post/clients/clients.h"
#include "endpoints/get/clients/clients.h"
#include "endpoints/post/services/services.h"
#include "endpoints/post/coupons/coupons.h"
#include "endpoints/post/orders/orders.h"
#include "endpoints/get/coupons/coupons.h"
#include "endpoints/get/services/services.h"
#include "endpoints/get/orders/orders.h"



using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;



PGconn* conn = nullptr;

const char *conninfo = "dbname=atelie user=postgres password=1234 hostaddr=127.0.0.1 port=5432";

int main() {
    http_listener listener("http://79.137.197.245:8080");

    listener.support(methods::POST, [](const http_request& request) {

        http_response response(status_codes::OK);
        response.headers().add(U("Access-Control-Allow-Origin"), U("*"));

        auto relative_path = uri::decode(request.relative_uri().path());
        if (relative_path == "/client") {
            request.extract_json().then([=](json::value request_body) {
                string client_name = request_body["client_name"].as_string();
                string client_phone = request_body["client_phone"].as_string();
                string client_address = request_body["client_address"].as_string();
                string client_email = request_body["client_email"].as_string();

                create_client(client_name, client_phone, client_address, client_email);

                request.reply(status_codes::OK);
            }).wait();
        } else if (relative_path == "/service") {
            request.extract_json().then([=](json::value request_body) {
                string service_name = request_body["service_name"].as_string();
                int service_price = request_body["service_price"].as_integer();

                create_service(service_name, service_price);

                request.reply(response);
            }).wait();
        } else if (relative_path == "/coupon") {
            request.extract_json().then([=](json::value request_body) {
                string coupon_name = request_body["coupon_name"].as_string();
                int coupon_discount = request_body["coupon_discount"].as_integer();

                create_coupon(coupon_name, coupon_discount);

                request.reply(response);
            }).wait();
        } else if (relative_path == "/create-order") {
            request.extract_json().then([=](json::value request_body) {
                int order_client_id = request_body["order_client_id"].as_integer();
                string order_date = request_body["order_date"].as_string();
                int order_amount = request_body["order_amount"].as_integer();

                create_order(order_client_id, order_date, order_amount);

                request.reply(response);
            }).wait();
        } else if (relative_path == "/insert-service") {
            request.extract_json().then([=](json::value request_body) {
                int order_id = request_body["order_id"].as_integer();
                int service_id = request_body["service_id"].as_integer();
                int service_amount = request_body["service_amount"].as_integer();

                int coupon_id = request_body["coupon_id"].is_null() ? -1 : request_body["coupon_id"].as_integer();
                insert_service(order_id, service_id, (coupon_id == -1) ? nullptr : &coupon_id, service_amount);

                request.reply(response);
            }).wait();
        } else if (relative_path == "/complete-order") {
            request.extract_json().then([=](json::value request_body) {
                int order_id = request_body["order_id"].as_integer();
                complete_order(order_id);

                request.reply(response);
            }).wait();
        } else {
            request.reply(status_codes::NotFound);
        }
    });

    listener.support(methods::GET, [](const http_request& request) {

        http_response response(status_codes::OK);
        response.headers().add(U("Access-Control-Allow-Origin"), U("*"));
    
        auto relative_path = uri::decode(request.relative_uri().path());
        if (relative_path == "/clients") {
            json::value response_body = get_all_clients();

            response.set_body(response_body);
            
            request.reply(response);
        } else if (relative_path == "/coupons") {
            json::value response_body = get_all_coupons();
            
            response.set_body(response_body);
            
            request.reply(response);
        } else if (relative_path == "/services") {
            json::value response_body = get_all_services();
            
            response.set_body(response_body);
            
            request.reply(response);
        } else if (relative_path == "/orders") {
            json::value response_body = get_all_orders();
            
            response.set_body(response_body);
            
            request.reply(response);
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

// g++ main.cpp endpoints/*/*/*.cpp  -o main -lboost_system -lcpprest -lssl -lcrypto -I/usr/include -lpq
