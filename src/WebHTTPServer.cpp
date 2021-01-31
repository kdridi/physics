//
//  WebHTTPServer.cpp
//  GGJ2021
//
//  Created by Karim DRIDI on 30/01/2021.
//

#include "WebHTTPServer.hpp"
#include "FileUtils.hpp"

#include <ixwebsocket/IXHttpServer.h>
#include <iostream>

WebHTTPServer::WebHTTPServer()
{
}

WebHTTPServer::~WebHTTPServer()
{
}

void WebHTTPServer::operator()(int port, const char *hostname)
{
    std::cout << "HTTP Listening on " << hostname << ":" << port << std::endl;

    ix::HttpServer server(port, hostname);
    server.setOnConnectionCallback([](ix::HttpRequestPtr request, std::shared_ptr<ix::ConnectionState> state) -> ix::HttpResponsePtr {
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
            headers["Content-Type"] = "text/html";

            std::string payload = FileUtils::GetInstance().getFileContent("/public/index.html");

            auto response = std::make_shared<ix::HttpResponse>(200, "OK", ix::HttpErrorCode::Ok, headers, payload);
            return response;
        }

        if (request->uri.compare("/js/p5/p5.js") == 0)
        {
            ix::WebSocketHttpHeaders headers{};
            headers["Content-Type"] = "text/javascript;charset=UTF-8: PASS";

            std::string payload = FileUtils::GetInstance().getFileContent("/public/js/p5/p5.js");

            auto response = std::make_shared<ix::HttpResponse>(200, "OK", ix::HttpErrorCode::Ok, headers, payload);
            return response;
        }

        if (request->uri.compare("/js/p5/addons/p5.sound.min.js") == 0)
        {
            ix::WebSocketHttpHeaders headers{};
            headers["Content-Type"] = "text/javascript;charset=UTF-8: PASS";

            std::string payload = FileUtils::GetInstance().getFileContent("/public/js/p5/addons/p5.sound.min.js");

            auto response = std::make_shared<ix::HttpResponse>(200, "OK", ix::HttpErrorCode::Ok, headers, payload);
            return response;
        }

        if (request->uri.compare("/js/sketch.js") == 0)
        {
            ix::WebSocketHttpHeaders headers{};
            headers["Content-Type"] = "text/javascript;charset=UTF-8: PASS";

            std::string payload = FileUtils::GetInstance().getFileContent("/public/js/sketch.js");

            auto response = std::make_shared<ix::HttpResponse>(200, "OK", ix::HttpErrorCode::Ok, headers, payload);
            return response;
        }

        if (request->uri.compare("/proto/box2d.proto") == 0)
        {
            ix::WebSocketHttpHeaders headers{};
            headers["Content-Type"] = "text/plain;charset=UTF-8";

            std::string payload = FileUtils::GetInstance().getFileContent("/public/proto/box2d.proto");

            auto response = std::make_shared<ix::HttpResponse>(200, "OK", ix::HttpErrorCode::Ok, headers, payload);
            return response;
        }

        if (true)
        {
            ix::WebSocketHttpHeaders headers{};
            headers["Content-Type"] = "text/html";

            std::string payload = "<html><head><title>404 NOT FOUND!</title></head><body>404 NOT FOUND!</body></html>";

            auto response = std::make_shared<ix::HttpResponse>(404, "Not Found", ix::HttpErrorCode::Ok, headers, payload);
            return response;
        }
    });

    auto res = server.listen();
    if (!res.first)
    {
        std::cout << res.second << std::endl;
    }
    else
    {
        server.start();
        server.wait();
    }
}
