//
//  WebSocketMessageCallback.hpp
//  GGJ2021
//
//  Created by Karim DRIDI on 30/01/2021.
//

#ifndef WebSocketMessageCallback_hpp
#define WebSocketMessageCallback_hpp

#include "GameManager.hpp"
#include "WebSocketClient.hpp"

class WebSocketClientManager
{
public:
    WebSocketClientManager();
    ~WebSocketClientManager();

    WebSocketClient &getClient(const std::string &key, std::shared_ptr<ix::WebSocket> ws);

    WebSocketClientManager &start(void);
    WebSocketClientManager &wait(void);

    WebSocketClientManager &removeClient(std::string const &key);
    size_t broadcast(const std::string &message);

    GameManager &gameManager(void)
    {
        return _gameManager;
    }

private:
    GameManager _gameManager;
    bool _score;
    std::mutex _mutex;
    std::map<std::string, std::shared_ptr<WebSocketClient>> _clients;
};

#endif /* WebSocketMessageCallback_hpp */
