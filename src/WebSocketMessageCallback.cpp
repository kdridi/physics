//
//  WebSocketMessageCallback.cpp
//  GGJ2021
//
//  Created by Karim DRIDI on 30/01/2021.
//

#include "WebSocketMessageCallback.hpp"

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

size_t WebSocketClient::sendMessage(std::string const& message) {
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
    if (_world != nullptr) {
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

WebSocketClientManager &WebSocketClientManager::removeClient(std::string const& key)
{
    const std::lock_guard<std::mutex> lock(_mutex);

    auto it = _clients.find(key);
    assert(it != _clients.end());
    _clients.erase(it);
    
    return *this;
}

size_t WebSocketClientManager::broadcast(const std::string& message) {
    const std::lock_guard<std::mutex> lock(_mutex);
    
    size_t result = 0;
    for (auto client : _clients) {
        result += client.second->sendMessage(message);
    }
    
    std::cout << "update world : " << result << ", count: " << _clients.size() << std::endl;

    return result;
}
