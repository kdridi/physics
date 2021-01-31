//
//  GameObject.hpp
//  GGJ2021
//
//  Created by Karim DRIDI on 31/01/2021.
//

#ifndef GameObject_hpp
#define GameObject_hpp

#include "GameManager.hpp"

class GameObject
{
public:
    GameObject(GameManager &manager, b2Body &body)
        : _manager(manager), _body(body)
    {
    }

    virtual ~GameObject()
    {
        _manager._world->DestroyBody(&_body);
    }

    b2Vec2 position() { return _body.GetPosition(); }

protected:
    GameManager &_manager;
    b2Body &_body;
};

#endif /* GameObject_hpp */
