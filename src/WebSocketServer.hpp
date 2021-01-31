//
//  WebSocketServer.hpp
//  GGJ2021
//
//  Created by Karim DRIDI on 30/01/2021.
//

#ifndef WebSocketServer_hpp
#define WebSocketServer_hpp

#include "WebSocketClientManager.hpp"

class WebSocketServer
{
public:
    WebSocketServer(WebSocketClientManager &manager);
    ~WebSocketServer();

    void operator()(int port, const char *hostname);
    void operator()(std::weak_ptr<ix::WebSocket> ptr, std::shared_ptr<ix::ConnectionState> st);

private:
    WebSocketClientManager &_manager;
};

#endif /* WebSocketServer_hpp */
