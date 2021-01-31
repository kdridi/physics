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
        float W = 1000.0f;
        ggj2021::Mouse mouse;

        const std::string prefix{"kp:"};
        if (msg->str.compare(0, prefix.size(), prefix.c_str()) == 0)
        {
            const std::string suffix{msg->str.c_str() + prefix.size()};
            std::cout << "KEY: [" << suffix << "]" << std::endl;
            if (suffix.compare("ArrowDown") == 0)
                _player->move(0.0f, 0.0f + W);
            else if (suffix.compare("ArrowUp") == 0)
                _player->move(0.0f, 0.0f - W);
            else if (suffix.compare("ArrowLeft") == 0)
                _player->move(0.0f - W, 0.0f);
            else if (suffix.compare("ArrowRight") == 0)
                _player->move(0.0f + W, 0.0f);
        }
        else if (parseMouse(msg, mouse))
        {
            b2Vec2 m{mouse.position().x(), mouse.position().y()};
            b2Vec2 p{_player->position()};
            _player->move(W * (m.x - p.x), W * (m.y - p.y));
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
