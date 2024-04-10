#include <iostream>
#include <string>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>

using namespace std;
using namespace web;
using namespace web::http;
using namespace web::http::experimental::listener;

const string endpoint = "http://localhost:8080/specific_endpoint";

void handle_specific_endpoint(const http_request& request) {
    http_response response(status_codes::OK);
    response.set_body("This is the specific endpoint response.");
    request.reply(response);
}

int main() {
    http_listener listener("http://localhost:8080");

    listener.support(methods::GET, [](const http_request& request) {
        auto relative_path = uri::decode(request.relative_uri().path());
        if (relative_path == "/specific_endpoint") {
            handle_specific_endpoint(request);
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
