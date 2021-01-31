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
public:
    GOPlayer(GameManager &manager, b2Body &body)
        : GameObject(manager, body)
    {
    }

    GOPlayer &move(float x, float y);

public:
    static GOPlayer *create(GameManager &manager);
};

#endif /* GOPlayer_hpp */
