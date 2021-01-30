//
//  WebHTTPServer.hpp
//  GGJ2021
//
//  Created by Karim DRIDI on 30/01/2021.
//

#ifndef WebHTTPServer_hpp
#define WebHTTPServer_hpp

class WebHTTPServer {
public:
    WebHTTPServer();
    ~WebHTTPServer();
    
    void operator()(int port, const char *hostname);
};

#endif /* WebHTTPServer_hpp */
