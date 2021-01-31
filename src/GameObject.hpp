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
    GameObject(GameManager &manager, b2Body &body, EntityCategory type)
        : _manager(manager), _body(body), _type(type)
    {
        _body.GetUserData().pointer = (uintptr_t) this;
    }

    virtual ~GameObject()
    {
        _manager._world->DestroyBody(&_body);
    }

    b2Vec2 position() { return _body.GetPosition(); }
    bool is(b2Body *body) { return &_body == body; }
    
public:
    static GameObject *as(b2Body *body) {
        if (body->GetUserData().pointer == 0)
            return nullptr;
        return (GameObject *) body->GetUserData().pointer;
    }
    
    template<class T>
    static T *as(b2Body *body, EntityCategory type) {
        GameObject *object = as(body);
        if (object == nullptr || object->_type != type)
            return nullptr;
        return dynamic_cast<T*>(object);
    }

public:
    EntityCategory _type;
    
protected:
    GameManager &_manager;
    b2Body &_body;
};

#endif /* GameObject_hpp */
