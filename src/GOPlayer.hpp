//
//  GOPlayer.hpp
//  GGJ2021
//
//  Created by Karim DRIDI on 31/01/2021.
//

#ifndef GOPlayer_hpp
#define GOPlayer_hpp

#include "GameObject.hpp"

class GOPlayer : public GameObject
{
private:
    GOPlayer(GameManager &manager, b2Body &body)
        : GameObject(manager, body, PLAYER)
    {
    }
    
public:
    static GOPlayer *as(b2Body *body) {
        return GameObject::as<GOPlayer>(body, PLAYER);
    }
    
public:
    GOPlayer &move(float x, float y);

public:
    static GOPlayer *create(GameManager &manager, bool left);
    
public:
    bool active = true;
    int score = 0;
    std::string name{"NONAME"};
};

#endif /* GOPlayer_hpp */
