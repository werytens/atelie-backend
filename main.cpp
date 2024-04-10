#include <iostream>
#include <string>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <jwt/jwt.hpp>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;
using namespace jwt::params;

const string endpoint = "http://localhost:8080/specific_endpoint";

// Secret key for JWT token signing
const string secret_key = "your_secret_key";

void handle_specific_endpoint(const http_request& request) {
    http_response response(status_codes::OK);
    response.set_body("This is the specific endpoint response.");
    request.reply(response);
}

void handle_jwt_generation(const http_request& request) {
    using namespace jwt::params;

    auto key = "secret"; //Secret to use for the algorithm
    //Create JWT object
    jwt::jwt_object obj{algorithm("HS256"), payload({{"some", "payload"}}), secret(key)};

    //Get the encoded string/assertion
    auto enc_str = obj.signature();
    std::cout << enc_str << std::endl;

    //Decode
    auto dec_obj = jwt::decode(enc_str, algorithms({"HS256"}), secret(key));
    std::cout << dec_obj.header() << std::endl;
    std::cout << dec_obj.payload() << std::endl;

    // Set response body with the generated token
    http_response response(status_codes::OK);
    response.headers().set_content_type("application/json");
    response.set_body(enc_str);
    request.reply(response);
}

int main() {
    http_listener listener("http://localhost:8080");

    listener.support(methods::GET, [](const http_request& request) {
        auto relative_path = uri::decode(request.relative_uri().path());
        if (relative_path == "/specific_endpoint") {
            handle_specific_endpoint(request);
        } else if (relative_path == "/jwt") {
            handle_jwt_generation(request);
        } else {
            request.reply(status_codes::NotFound);
        }
    });

    try {
        listener.open().then([&listener]() {
            cout << "Server is listening on http://localhost:8080" << endl;
        }).wait();

        while (true);
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}


// compile:  g++ main.cpp -o main -lboost_system -lcpprest -lssl -lcrypto