//
//  GOGoal.hpp
//  GGJ2021
//
//  Created by Karim DRIDI on 31/01/2021.
//

#ifndef GOGoal_hpp
#define GOGoal_hpp

#include "GameObject.hpp"

class GOGoal : public GameObject
{
private:
    GOGoal(GameManager &manager, b2Body &body)
        : GameObject(manager, body, GOAL)
    {
    }
    
public:
    static GOGoal *as(b2Body *body) {
        return GameObject::as<GOGoal>(body, GOAL);
    }

public:
    static GOGoal *create(GameManager &manager, float x, float y, float w, float h);
};

#endif /* GOGoal_hpp */
