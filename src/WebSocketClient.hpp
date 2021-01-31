//
//  WebSocketClient.hpp
//  GGJ2021
//
//  Created by Karim DRIDI on 31/01/2021.
//

#ifndef WebSocketClient_hpp
#define WebSocketClient_hpp

#include "GOPlayer.hpp"

#include <ixwebsocket/IXWebSocketServer.h>

class WebSocketClientManager;

class WebSocketClient
{
public:
    WebSocketClient(std::string key, WebSocketClientManager &manager, std::shared_ptr<ix::WebSocket> socket);
    ~WebSocketClient();

    void operator()(const ix::WebSocketMessagePtr &msg);
    size_t sendMessage(std::string const &message);

private:
    std::string const _key;
    WebSocketClientManager &_manager;
    std::shared_ptr<ix::WebSocket> _socket;
    GOPlayer *_player;
};

#endif /* WebSocketClient_hpp */
