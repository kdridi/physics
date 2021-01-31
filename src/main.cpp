#include "WebHTTPServer.hpp"
#include "WebSocketServer.hpp"
#include "WebSocketClientManager.hpp"

int main()
{
    WebSocketClientManager manager;
    WebHTTPServer httpServer;
    WebSocketServer socketServer{manager};

    std::thread http{httpServer, 9090, "0.0.0.0"};
    std::thread ws{socketServer, 9091, "0.0.0.0"};
    std::thread world{[&manager]() {
        manager.start();
        manager.wait();
    }};

    world.join();
    ws.join();
    http.join();

    return (0);
}
