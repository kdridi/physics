//
//  WebSocketMessageCallback.cpp
//  GGJ2021
//
//  Created by Karim DRIDI on 30/01/2021.
//

#include "WebSocketMessageCallback.hpp"
#include "WebSocketSerializer.hpp"
#include "proto/box2d.pb.h"

#include <sstream>
#include <iostream>

WebSocketClient::WebSocketClient(std::string key, WebSocketClientManager &manager, std::shared_ptr<ix::WebSocket> socket)
    : _key(key), _manager(manager), _socket(socket), _body(nullptr)
{
}

WebSocketClient::~WebSocketClient()
{
}

static bool parseMouse(const ix::WebSocketMessagePtr &msg, ggj2021::Mouse &mouse)
{
    const std::string prefix{"mp:"};
    if (msg->str.compare(0, prefix.size(), prefix.c_str()) != 0)
        return false;

    std::string s = msg->str;
    std::string delimiter = ":";

    auto position = mouse.mutable_position();

    size_t pos = 0;
    std::string token;
    int i = 0;
    while ((pos = s.find(delimiter)) != std::string::npos)
    {
        token = s.substr(0, pos);
        if (i != 0)
        {
            int t = std::stoi(token);
            switch (i)
            {
            case 1:
                mouse.set_left(t == 1);
                break;
            case 2:
                mouse.set_right(t == 1);
                break;
            case 3:
                position->set_x(t);
                break;
            case 4:
                position->set_y(t);
                break;
            default:
                break;
            }
        }
        s.erase(0, pos + delimiter.length());
        i += 1;
    }

    return true;
}

void WebSocketClient::operator()(const ix::WebSocketMessagePtr &msg)
{
    switch (msg->type)
    {
    case ix::WebSocketMessageType::Message:
    {
        ggj2021::Mouse mouse;

        const std::string prefix{"kp:"};
        if (msg->str.compare(0, prefix.size(), prefix.c_str()) == 0)
        {
            float power = 800000.0f;
            const std::string suffix{msg->str.c_str() + prefix.size()};
            std::cout << "KEY: [" << suffix << "]" << std::endl;
            if (suffix.compare("ArrowDown") == 0)
            {
                b2Vec2 force{0.0f, 0.0f + power};
                b2Vec2 point = _body->GetPosition();
                _body->ApplyLinearImpulseToCenter(force, true);
            }
            else if (suffix.compare("ArrowUp") == 0)
            {
                b2Vec2 force{0.0f, 0.0f - power};
                b2Vec2 point = _body->GetPosition();
                _body->ApplyLinearImpulseToCenter(force, true);
            }
            else if (suffix.compare("ArrowLeft") == 0)
            {
                b2Vec2 force{0.0f - power, 0.0f};
                b2Vec2 point = _body->GetPosition();
                _body->ApplyLinearImpulseToCenter(force, true);
            }
            else if (suffix.compare("ArrowRight") == 0)
            {
                b2Vec2 force{0.0f + power, 0.0f};
                b2Vec2 point = _body->GetPosition();
                _body->ApplyLinearImpulseToCenter(force, true);
            }
        }
        else if (parseMouse(msg, mouse))
        {
            std::cout
            << "left: " << mouse.left() << std::endl
            << "right: " << mouse.right() << std::endl
            << "x: " << mouse.position().x() << std::endl
            << "y: " << mouse.position().y() << std::endl;
            
            float w = 80000000.0f;
            b2Vec2 m{mouse.position().x(), mouse.position().y()};
            b2Vec2 p{_body->GetPosition()};
            b2Vec2 f{m.x - p.x, m.y - p.y};
            f *= w;
            _body->ApplyLinearImpulseToCenter(f, true);
        }
        break;
    }
    case ix::WebSocketMessageType::Open:
    {
#pragma mark spawn the player
        b2PolygonShape shape;
        shape.SetAsBox(10.0, 10.0f);

        float px = 100.0 * (2.0 * ((double)rand() / (RAND_MAX)) - 1.0);
        px += px > 0 ? 0.0 + 100.0 : 0.0 - 100.0f;

        float py = 100.0 * (2.0 * ((double)rand() / (RAND_MAX)) - 1.0);
        py += py > 0 ? 0.0 + 100.0 : 0.0 - 100.0f;

        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.position.Set(px, py);
        _body = _manager.world().CreateBody(&bd);
        _body->CreateFixture(&shape, 2.0f);
        _body->SetLinearDamping(0.0f);

        break;
    }
    case ix::WebSocketMessageType::Close:
    {
        _manager.world().DestroyBody(_body);
        _manager.removeClient(_key);
        break;
    }
    default:
    {
        assert(false);
        break;
    }
    }
}

size_t WebSocketClient::sendMessage(std::string const &message)
{
    _socket->sendBinary(message);
    return message.size();
}

WebSocketClientManager::WebSocketClientManager()
    : _mutex(), _clients(), _world(nullptr), _ball(nullptr), _dimensions(800.0f, 600.0f)
{
    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    _world = new b2World{gravity};
}

WebSocketClientManager::~WebSocketClientManager()
{
    if (_world != nullptr)
    {
        delete _world;
    }
}

WebSocketClient &WebSocketClientManager::getClient(const std::string &key, std::shared_ptr<ix::WebSocket> ws)
{
    const std::lock_guard<std::mutex> lock(_mutex);

    auto it = _clients.find(key);
    if (it == _clients.end())
        _clients[key] = std::make_shared<WebSocketClient>(key, *this, ws);
    assert(_clients.at(key) != nullptr);
    return *(_clients.at(key));
}

WebSocketClientManager &WebSocketClientManager::removeClient(std::string const &key)
{
    const std::lock_guard<std::mutex> lock(_mutex);

    auto it = _clients.find(key);
    assert(it != _clients.end());
    _clients.erase(it);

    return *this;
}

size_t WebSocketClientManager::broadcast(const std::string &message)
{
    const std::lock_guard<std::mutex> lock(_mutex);

    size_t result = 0;
    for (auto client : _clients)
    {
        result += client.second->sendMessage(message);
    }

    //    std::cout << "update world : " << result << ", count: " << _clients.size() << std::endl;

    return result;
}

WebSocketClientManager &WebSocketClientManager::start(void)
{
    static const float width = 20.0f;

#pragma mark spawn the field
    // border bottom
    {
        b2Vec2 dim{_dimensions.x, width};

        b2PolygonShape shape;
        shape.SetAsBox(dim.x / 2, dim.y / 2);

        b2BodyDef bd;
        bd.type = b2_staticBody;
        bd.position.Set(0.0f, 0 + (_dimensions.y - dim.y) / 2.0);

        b2Body *body = _world->CreateBody(&bd);
        body->CreateFixture(&shape, 2.0f);
    }
    // border bottom
    {
        b2Vec2 dim{_dimensions.x, width};

        b2PolygonShape shape;
        shape.SetAsBox(dim.x / 2, dim.y / 2);

        b2BodyDef bd;
        bd.type = b2_staticBody;
        bd.position.Set(0.0f, 0 - (_dimensions.y - dim.y) / 2.0);

        b2Body *body = _world->CreateBody(&bd);
        body->CreateFixture(&shape, 2.0f);
    }
    // border left
    {
        b2Vec2 dim{width, _dimensions.y - 2.0f * width};

        b2PolygonShape shape;
        shape.SetAsBox(dim.x / 2, dim.y / 2);

        b2BodyDef bd;
        bd.type = b2_staticBody;
        bd.position.Set(0 - (_dimensions.x - dim.x) / 2.0, 0.0f);

        b2Body *body = _world->CreateBody(&bd);
        body->CreateFixture(&shape, 2.0f);
    }
    // border left
    {
        b2Vec2 dim{width, _dimensions.y - 2.0f * width};

        b2PolygonShape shape;
        shape.SetAsBox(dim.x / 2, dim.y / 2);

        b2BodyDef bd;
        bd.type = b2_staticBody;
        bd.position.Set(0 + (_dimensions.x - dim.x) / 2.0, 0.0f);

        b2Body *body = _world->CreateBody(&bd);
        body->CreateFixture(&shape, 2.0f);
    }
#pragma mark spawn the ball
    {
        b2CircleShape shape;
        shape.m_radius = width;

        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.position.Set(0.0f, 0.0f);
        _ball = _world->CreateBody(&bd);
        _ball->CreateFixture(&shape, 2.0f);
    }

    return *this;
}

WebSocketClientManager &WebSocketClientManager::wait(void)
{
    WebSocketSerializer serializer{};

    bool running = true;
    while (running)
    {
        size_t size = broadcast(serializer.serialize(world()));
        world().Step(.030f, 10, 10);
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    return *this;
}
