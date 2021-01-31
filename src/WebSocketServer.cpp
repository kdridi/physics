//
//  WebSocketServer.cpp
//  GGJ2021
//
//  Created by Karim DRIDI on 30/01/2021.
//

#include "WebSocketServer.hpp"

#include <ixwebsocket/IXWebSocketServer.h>
#include <sstream>
#include <iostream>

WebSocketServer::WebSocketServer(WebSocketClientManager &manager)
    : _manager(manager)
{
}

WebSocketServer::~WebSocketServer()
{
}

void WebSocketServer::operator()(std::weak_ptr<ix::WebSocket> ptr, std::shared_ptr<ix::ConnectionState> st)
{
    if (auto ws = ptr.lock())
    {
        std::stringstream key;
        key << st->getRemoteIp();
        key << ":";
        key << st->getRemotePort();

        ws->setOnMessageCallback(_manager.getClient(key.str(), ws));
    }
}

void WebSocketServer::operator()(int port, const char *hostname)
{
    std::cout << "WS Listening on " << hostname << ":" << port << std::endl;

    ix::WebSocketServer server(port, hostname);
    server.setOnConnectionCallback(*this);

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
