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

static std::map<std::string, std::shared_ptr<ix::WebSocket>> clients;
// static std::map<std::weak_ptr<ix::WebSocket>, std::list<std::string> *> actions;

static void wsStart(int port, const char *hostname)
{
    std::cout << "WS Listening on " << hostname << ":" << port << std::endl;

    ix::WebSocketServer server(port, hostname);
    server.setOnConnectionCallback([](std::weak_ptr<ix::WebSocket> ptr, std::shared_ptr<ix::ConnectionState> st) {
        if (auto ws = ptr.lock())
        {
            ws->setOnMessageCallback([st, ws](const ix::WebSocketMessagePtr &msg) -> void {
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
                    std::string key{msg->str.c_str()};
                    std::cout << "received Open: " << std::endl;
                    std::cout << "State ID: " << st->getId() << std::endl;
                    std::cout << "\tbinary: " << (msg->binary ? "true" : "false") << std::endl;
                    std::cout << "\tstr: " << msg->str << std::endl;
                    std::cout << "\turi: " << msg->openInfo.uri << std::endl;
                    std::cout << "\tprotocol: " << msg->openInfo.protocol << std::endl;
                    for (auto header : msg->openInfo.headers)
                        std::cout << "\theader[\"" << header.first << "\"] = \"" << header.second << "\"" << std::endl;
                    // actions[ptr] = new std::list<std::string>{};
                    clients[key] = ws;
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

#include "proto/box2d.pb.h"

class Draw : public b2Draw
{
public:
    Draw()
    {
    }

    virtual ~Draw()
    {
    }

public:
    virtual void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
    {
        std::cout << "DrawPolygon" << std::endl;
    }

    virtual void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
    {
        std::cout << "DrawSolidPolygon" << std::endl;
    }

    virtual void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color)
    {
        std::cout << "DrawCircle" << std::endl;
    }

    virtual void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color)
    {
        std::cout << "DrawSolidCircle" << std::endl;
    }

    virtual void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
    {
        std::cout << "DrawSegment" << std::endl;
    }

    virtual void DrawTransform(const b2Transform &xf)
    {
        std::cout << "DrawTransform" << std::endl;
    }

    virtual void DrawPoint(const b2Vec2 &p, float size, const b2Color &color)
    {
        std::cout << "DrawPoint" << std::endl;
    }
};

static void worldStart(void)
{
    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
    b2World world{gravity};

    b2Body *player = nullptr;

    b2Body *ground = NULL;
    {
        b2BodyDef bd;
        ground = world.CreateBody(&bd);

        b2EdgeShape shape;
        shape.SetTwoSided(b2Vec2(-40.0f, 0.0f), b2Vec2(40.0f, 0.0f));
        ground->CreateFixture(&shape, 0.0f);
    }

    {
        b2Body *prevBody = ground;

        // Define crank.
        {
            b2PolygonShape shape;
            shape.SetAsBox(0.5f, 2.0f);

            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.position.Set(0.0f, 7.0f);
            b2Body *body = world.CreateBody(&bd);
            body->CreateFixture(&shape, 2.0f);

            b2RevoluteJointDef rjd;
            rjd.Initialize(prevBody, body, b2Vec2(0.0f, 5.0f));
            rjd.motorSpeed = 1.0f * b2_pi;
            rjd.maxMotorTorque = 10000.0f;
            rjd.enableMotor = true;
            b2RevoluteJoint *m_joint1 = (b2RevoluteJoint *)world.CreateJoint(&rjd);

            prevBody = body;
        }

        // Define follower.
        {
            b2PolygonShape shape;
            shape.SetAsBox(0.5f, 4.0f);

            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.position.Set(0.0f, 13.0f);
            b2Body *body = world.CreateBody(&bd);
            body->CreateFixture(&shape, 2.0f);

            b2RevoluteJointDef rjd;
            rjd.Initialize(prevBody, body, b2Vec2(0.0f, 9.0f));
            rjd.enableMotor = false;
            world.CreateJoint(&rjd);

            prevBody = body;
        }

        // Define piston
        {
            b2PolygonShape shape;
            shape.SetAsBox(1.5f, 1.5f);

            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.fixedRotation = true;
            bd.position.Set(0.0f, 17.0f);
            b2Body *body = world.CreateBody(&bd);
            body->CreateFixture(&shape, 2.0f);

            b2RevoluteJointDef rjd;
            rjd.Initialize(prevBody, body, b2Vec2(0.0f, 17.0f));
            world.CreateJoint(&rjd);

            b2PrismaticJointDef pjd;
            pjd.Initialize(ground, body, b2Vec2(0.0f, 17.0f), b2Vec2(0.0f, 1.0f));

            pjd.maxMotorForce = 1000.0f;
            pjd.enableMotor = true;

            b2PrismaticJoint *m_joint2 = (b2PrismaticJoint *)world.CreateJoint(&pjd);
        }

        // Create a payload
        {
            b2PolygonShape shape;
            shape.SetAsBox(1.5f, 1.5f);

            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.position.Set(0.0f, 23.0f);
            b2Body *body = world.CreateBody(&bd);
            body->CreateFixture(&shape, 2.0f);

            player = body;
        }
    }

    Draw debugDraw{};
    world.SetDebugDraw(&debugDraw);
    debugDraw.SetFlags(b2Draw::e_shapeBit);

    ggj2021::Player p;

    bool running = true;
    while (running)
    {
        const b2Vec2 &position = player->GetPosition();
        p.set_x(position.x);
        p.set_y(position.y);
        p.set_msg("Hello, world!");

        std::stringstream ss;
        ss << position.x << std::endl
           << position.y << std::endl;

        //        world.DebugDraw();

        for (auto client : clients)
            client.second->sendBinary(p.SerializeAsString());

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
