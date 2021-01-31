//
//  WebSocketClient.cpp
//  GGJ2021
//
//  Created by Karim DRIDI on 31/01/2021.
//

#include "WebSocketClient.hpp"
#include "WebSocketClientManager.hpp"
#include "proto/box2d.pb.h"

WebSocketClient::WebSocketClient(std::string key, WebSocketClientManager &manager, std::shared_ptr<ix::WebSocket> socket)
    : _key(key), _manager(manager), _socket(socket), _player(nullptr)
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
        ggj2021::c2s message;
        message.ParseFromString(msg->str);
        
        float W = 1000.0f;
        if (message.keyboard().compare("ArrowDown") == 0)
            _player->move(0.0f, 0.0f + W);
        else if (message.keyboard().compare("ArrowUp") == 0)
            _player->move(0.0f, 0.0f - W);
        else if (message.keyboard().compare("ArrowLeft") == 0)
            _player->move(0.0f - W, 0.0f);
        else if (message.keyboard().compare("ArrowRight") == 0)
            _player->move(0.0f + W, 0.0f);

        auto mouse = message.mouse();
        if (mouse.left() == true) {
            auto position = mouse.position();
            b2Vec2 m{position.x(), position.y()};
            b2Vec2 p{_player->position()};
            _player->move(W * (m.x - p.x), W * (m.y - p.y));
        }
        
        auto playerName = message.playername();
        if (playerName.compare("") != 0) {
            _player->name = playerName;
        }

        break;
    }
    case ix::WebSocketMessageType::Open:
    {
        _player = _manager.gameManager().createPlayer();
        break;
    }
    case ix::WebSocketMessageType::Close:
    {
        _manager.gameManager().destroyPlayer(_player);
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
