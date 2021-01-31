//
//  WebSocketClientManager.cpp
//  GGJ2021
//
//  Created by Karim DRIDI on 30/01/2021.
//

#include "WebSocketClientManager.hpp"
#include "WebSocketSerializer.hpp"

#include "GOPlayer.hpp"

#include <sstream>
#include <iostream>

WebSocketClientManager::WebSocketClientManager()
    : _score(true), _mutex(), _clients()
{
}

WebSocketClientManager::~WebSocketClientManager()
{
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
        result += client.second->sendMessage(message);
    //    std::cout << "update world : " << result << ", count: " << _clients.size() << std::endl;
    return result;
}

WebSocketClientManager &WebSocketClientManager::start(void)
{
    _gameManager.createGame();
    return *this;
}

WebSocketClientManager &WebSocketClientManager::wait(void)
{
    bool running = true;
    while (running)
    {
        size_t size = broadcast(_gameManager.updateGame());
        std::this_thread::sleep_for(std::chrono::milliseconds(30));
    }

    return *this;
}
