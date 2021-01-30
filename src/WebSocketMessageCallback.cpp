//
//  WebSocketMessageCallback.cpp
//  GGJ2021
//
//  Created by Karim DRIDI on 30/01/2021.
//

#include "WebSocketMessageCallback.hpp"
#include "WebSocketSerializer.hpp"

#include <sstream>
#include <iostream>

WebSocketClient::WebSocketClient(std::string key, WebSocketClientManager &manager, std::shared_ptr<ix::WebSocket> socket)
    : _key(key), _manager(manager), _socket(socket), _body(nullptr)
{
}

WebSocketClient::~WebSocketClient()
{
}

void WebSocketClient::operator()(const ix::WebSocketMessagePtr &msg)
{
    switch (msg->type)
    {
    case ix::WebSocketMessageType::Message:
    {
        const std::string prefix{"keyPressed: "};
        if (msg->str.compare(0, prefix.size(), prefix.c_str()) == 0)
        {
            const std::string suffix{msg->str.c_str() + prefix.size()};
            std::cout << "KEY: [" << suffix << "]" << std::endl;
            if (suffix.compare("ArrowUp") == 0)
            {
                b2Vec2 force{0.0f, 200.0f};
                b2Vec2 point = _body->GetPosition();
                _body->ApplyLinearImpulseToCenter(force, true);
            }
            else if (suffix.compare("ArrowLeft") == 0)
            {
                b2Vec2 force{-200.0f, 0.0f};
                b2Vec2 point = _body->GetPosition();
                _body->ApplyLinearImpulseToCenter(force, true);
            }
            else if (suffix.compare("ArrowRight") == 0)
            {
                b2Vec2 force{200.0f, 0.0f};
                b2Vec2 point = _body->GetPosition();
                _body->ApplyLinearImpulseToCenter(force, true);
            }
        }
        break;
    }
    case ix::WebSocketMessageType::Open:
    {
        b2PolygonShape shape;
        shape.SetAsBox(1.5f, 1.5f);

        b2BodyDef bd;
        bd.type = b2_dynamicBody;
        bd.position.Set(-10.0f, 23.0f);
        _body = _manager.world().CreateBody(&bd);
        _body->CreateFixture(&shape, 2.0f);
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
{
    b2Vec2 gravity;
    gravity.Set(0.0f, -10.0f);
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

    std::cout << "update world : " << result << ", count: " << _clients.size() << std::endl;

    return result;
}

WebSocketClientManager &WebSocketClientManager::start(void) {
    b2Body *ground = NULL;
    
    {
        b2BodyDef bd;
        ground = _world->CreateBody(&bd);

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
            b2Body *body = _world->CreateBody(&bd);
            body->CreateFixture(&shape, 2.0f);

            b2RevoluteJointDef rjd;
            rjd.Initialize(prevBody, body, b2Vec2(0.0f, 5.0f));
            rjd.motorSpeed = 1.0f * b2_pi;
            rjd.maxMotorTorque = 10000.0f;
            rjd.enableMotor = true;
            _world->CreateJoint(&rjd);

            prevBody = body;
        }

        // Define follower.
        {
            b2PolygonShape shape;
            shape.SetAsBox(0.5f, 4.0f);

            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.position.Set(0.0f, 13.0f);
            b2Body *body = _world->CreateBody(&bd);
            body->CreateFixture(&shape, 2.0f);

            b2RevoluteJointDef rjd;
            rjd.Initialize(prevBody, body, b2Vec2(0.0f, 9.0f));
            rjd.enableMotor = false;
            _world->CreateJoint(&rjd);

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
            b2Body *body = _world->CreateBody(&bd);
            body->CreateFixture(&shape, 2.0f);

            b2RevoluteJointDef rjd;
            rjd.Initialize(prevBody, body, b2Vec2(0.0f, 17.0f));
            _world->CreateJoint(&rjd);

            b2PrismaticJointDef pjd;
            pjd.Initialize(ground, body, b2Vec2(0.0f, 17.0f), b2Vec2(0.0f, 1.0f));

            pjd.maxMotorForce = 1000.0f;
            pjd.enableMotor = true;

            _world->CreateJoint(&pjd);
        }

        // Create a payload
        {
            b2PolygonShape shape;
            shape.SetAsBox(1.5f, 1.5f);

            b2BodyDef bd;
            bd.type = b2_dynamicBody;
            bd.position.Set(0.0f, 23.0f);
            b2Body *body = _world->CreateBody(&bd);
            body->CreateFixture(&shape, 2.0f);
        }
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
