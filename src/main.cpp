#include <ixwebsocket/IXHttpServer.h>
#include <sstream>
#include <iostream>

int main()
{
    int port;
    std::stringstream ss;
    ss << "9090";
    ss >> port;

    std::string hostname("0.0.0.0");

    std::cout << "Listening on " << hostname << ":" << port << std::endl;

    ix::HttpServer server(port, hostname);
    server.setOnConnectionCallback([](ix::HttpRequestPtr request, std::shared_ptr<ix::ConnectionState> state) -> ix::HttpResponsePtr {
//        std::cout << "request->uri    : " << request->uri << std::endl
//                  << "request->body   : " << request->body << std::endl
//                  << "request->version: " << request->version << std::endl
//                  << "state           : " << state << std::endl;

        if (request->uri.compare("/") == 0)
        {
            ix::WebSocketHttpHeaders headers{};
            headers["Location"] = "/index.html";

            auto response = std::make_shared<ix::HttpResponse>(301, "Moved Permanently", ix::HttpErrorCode::Ok, headers);
            return response;
        }

        if (request->uri.compare("/index.html") == 0)
        {
            ix::WebSocketHttpHeaders headers{};
            std::string payload = "<html><head><title>Hello</title></head><body>Hello, world!</body></html>";

            auto response = std::make_shared<ix::HttpResponse>(200, "OK", ix::HttpErrorCode::Ok, headers, payload);
            return response;
        }

        if (true)
        {
            ix::WebSocketHttpHeaders headers{};
            std::string payload = "<html><head><title>404 NOT FOUND!</title></head><body>404 NOT FOUND!</body></html>";

            auto response = std::make_shared<ix::HttpResponse>(404, "Not Found", ix::HttpErrorCode::Ok, headers, payload);
            return response;
        }
    });

    auto res = server.listen();
    if (!res.first)
    {
        std::cout << res.second << std::endl;
        return 1;
    }

    server.start();
    server.wait();

    return 0;
}
