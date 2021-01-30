//
//  WebSocketMessageCallback.hpp
//  GGJ2021
//
//  Created by Karim DRIDI on 30/01/2021.
//

#ifndef WebSocketMessageCallback_hpp
#define WebSocketMessageCallback_hpp

#include <ixwebsocket/IXWebSocketServer.h>
#include <box2d/box2d.h>
#include <map>

enum EntityCategory {
    BOUNDARY      =     0x0001,
    LTEAM         =     0x0002,
    RTEAM         =     0x0004,
    BALL          =     0x0008,
    GOAL          =     0x0010,
};

using WebSocketClients = std::map<std::string, std::shared_ptr<ix::WebSocket>>;
using b2Players = std::map<std::string, b2Body*>;

class WebSocketClientManager;

class WebSocketClient {
public:
    WebSocketClient(std::string key, WebSocketClientManager& manager, std::shared_ptr<ix::WebSocket> socket);
    ~WebSocketClient();
    
    void operator()(const ix::WebSocketMessagePtr &msg);
    size_t sendMessage(std::string const& message);
    
private:
    std::string const _key;
    WebSocketClientManager& _manager;
    std::shared_ptr<ix::WebSocket> _socket;
    b2Body *_body;
};

class WebSocketClientManager : public b2ContactListener {
public:
    WebSocketClientManager();
    ~WebSocketClientManager();
    
    WebSocketClient &getClient(const std::string& key, std::shared_ptr<ix::WebSocket> ws);

    b2World &world() {
        return *_world;
    }
    
    WebSocketClientManager &start(void);
    WebSocketClientManager &wait(void);

    WebSocketClientManager &removeClient(std::string const& key);
    size_t broadcast(const std::string& message);
    WebSocketClientManager &spawnNewBall();
    
public:
    virtual void BeginContact(b2Contact* contact);
    
private:
    bool _score;
    std::mutex _mutex;
    std::map<std::string, std::shared_ptr<WebSocketClient>> _clients;
    b2World *_world;
    b2Body *_ball;
    b2Body *_rgoal;
    b2Body *_lgoal;
    b2Vec2 _dimensions;
};

#endif /* WebSocketMessageCallback_hpp */
