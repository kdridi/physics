#include <ixwebsocket/IXHttpServer.h>
#include <ixwebsocket/IXWebSocketServer.h>
#include <sstream>
#include <iostream>
#include <thread>
#include <box2d/box2d.h>
#include <chrono>
#include <thread>
#include <map>
#include <list>

#include "file_utils.hpp"

static void httpStart(int port, const char *hostname)
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

static std::map<std::string, std::weak_ptr<ix::WebSocket>> clients;
// static std::map<std::weak_ptr<ix::WebSocket>, std::list<std::string> *> actions;

static void wsStart(int port, const char *hostname)
{
    std::cout << "WS Listening on " << hostname << ":" << port << std::endl;

    ix::WebSocketServer server(port, hostname);
    server.setOnConnectionCallback([](std::weak_ptr<ix::WebSocket> ptr, std::shared_ptr<ix::ConnectionState> st) {
        if (auto ws = ptr.lock())
        {
            ws->setOnMessageCallback([st, ptr](const ix::WebSocketMessagePtr &msg) -> void {
                switch (msg->type)
                {
                case ix::WebSocketMessageType::Message:
                {
                    std::cout << "received Message: " << std::endl;
                    std::cout << "State ID: " << st->getId() << std::endl;
                    std::cout << "\tbinary: " << (msg->binary ? "true" : "false") << std::endl;
                    std::cout << "\tstr: " << msg->str << std::endl;
                    // actions[ptr]->push_back(msg->str);
                    break;
                }
                case ix::WebSocketMessageType::Open:
                {
                    std::cout << "received Open: " << std::endl;
                    std::cout << "State ID: " << st->getId() << std::endl;
                    std::cout << "\tbinary: " << (msg->binary ? "true" : "false") << std::endl;
                    std::cout << "\tstr: " << msg->str << std::endl;
                    std::cout << "\turi: " << msg->openInfo.uri << std::endl;
                    std::cout << "\tprotocol: " << msg->openInfo.protocol << std::endl;
                    for (auto header : msg->openInfo.headers)
                        std::cout << "\theader[\"" << header.first << "\"] = \"" << header.second << "\"" << std::endl;
                    clients[msg->str] = ptr;
                    // actions[ptr] = new std::list<std::string>{};
                    break;
                }
                case ix::WebSocketMessageType::Close:
                {
                    std::cout << "received Close: " << std::endl;
                    std::cout << "State ID: " << st->getId() << std::endl;
                    std::cout << "\tbinary: " << (msg->binary ? "true" : "false") << std::endl;
                    std::cout << "\tstr: " << msg->str << std::endl;
                    std::cout << "\tcode: " << msg->closeInfo.code << std::endl;
                    std::cout << "\treason: " << msg->closeInfo.reason << std::endl;
                    std::cout << "\tremote: " << (msg->closeInfo.remote ? "true" : "false") << std::endl;
                    // actions.erase(clients[msg->str]);
                    // delete actions[clients[msg->str]];
                    // actions.erase(clients[msg->str]);
                    clients.erase(msg->str);
                    break;
                }
                case ix::WebSocketMessageType::Error:
                {
                    std::cout << "received Error: " << msg->str << std::endl;
                    std::cout << "State ID: " << st->getId() << std::endl;
                    std::cout << "\tbinary: " << msg->binary << std::endl;
                    std::cout << "\tstr: " << msg->str << std::endl;
                    std::cout << "\tretries: " << msg->errorInfo.retries << std::endl;
                    std::cout << "\twait_time: " << msg->errorInfo.wait_time << std::endl;
                    std::cout << "\thttp_status: " << msg->errorInfo.http_status << std::endl;
                    std::cout << "\treason: " << msg->errorInfo.reason << std::endl;
                    std::cout << "\tdecompressionError: " << msg->errorInfo.decompressionError << std::endl;
                    break;
                }
                case ix::WebSocketMessageType::Ping:
                {
                    std::cout << "received Ping: " << msg->str << std::endl;
                    std::cout << "State ID: " << st->getId() << std::endl;
                    std::cout << "\tbinary: " << msg->binary << std::endl;
                    std::cout << "\tstr: " << msg->str << std::endl;
                    break;
                }
                case ix::WebSocketMessageType::Pong:
                {
                    std::cout << "received Pong: " << msg->str << std::endl;
                    std::cout << "State ID: " << st->getId() << std::endl;
                    std::cout << "\tbinary: " << msg->binary << std::endl;
                    std::cout << "\tstr: " << msg->str << std::endl;
                    break;
                }
                case ix::WebSocketMessageType::Fragment:
                {
                    std::cout << "received Fragment: " << msg->str << std::endl;
                    std::cout << "State ID: " << st->getId() << std::endl;
                    std::cout << "\tbinary: " << msg->binary << std::endl;
                    std::cout << "\tstr: " << msg->str << std::endl;
                    break;
                }
                default:
                    break;
                }
            });
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

static void worldStart(void)
{
    b2Vec2 gravity{0.0, -9.8};
    b2World world{gravity};

    b2Body *player = nullptr;

    {
        b2BodyDef def;
        def.type = b2BodyType::b2_staticBody;
        def.position.Set(400.0f, 50.0f);

        b2PolygonShape shape;
        shape.SetAsBox(800.0f, 100.0f);

        b2Body *body = world.CreateBody(&def);
        body->CreateFixture(&shape, 0.0f);
    }

    {
        b2BodyDef def;
        def.type = b2BodyType::b2_dynamicBody;
        def.position.Set(400.0f, 300.0f);

        b2PolygonShape shape;
        shape.SetAsBox(1.0f, 1.0f);

        b2FixtureDef fixture;
        fixture.shape = &shape;
        fixture.density = 10.0f;
        fixture.friction = 0.3f;

        b2Body *body = world.CreateBody(&def);
        body->CreateFixture(&fixture);

        player = body;
    }

    bool running = true;
    while (running)
    {
        const b2Vec2 &position = player->GetPosition();

        std::stringstream ss;
        ss << position.x << std::endl
           << position.y << std::endl;

        for (auto client : clients)
        {
            if (auto ws = client.second.lock())
            {
                ws->sendText(ss.str());
            }
        }

        // std::cout << "update world" << std::endl;
        world.Step(.015f, 10, 10);
        std::this_thread::sleep_for(std::chrono::milliseconds(15));
    }
}

int main()
{
    std::thread http{httpStart, 9090, "0.0.0.0"};
    std::thread ws{wsStart, 9091, "0.0.0.0"};
    std::thread world{worldStart};

    world.join();
    ws.join();
    http.join();

    return (0);
}
