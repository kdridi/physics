//
//  GOBall.hpp
//  GGJ2021
//
//  Created by Karim DRIDI on 31/01/2021.
//

#ifndef GOBall_hpp
#define GOBall_hpp

#include "GameObject.hpp"

#include <list>
#include <mutex>

class GOPlayer;

class GOBall : public GameObject
{
private:
    GOBall(GameManager &manager, b2Body &body)
        : GameObject(manager, body, BALL)
    {
    }
    
public:
    static GOBall *as(b2Body *body) {
        return GameObject::as<GOBall>(body, BALL);
    }
    
public:
    GOBall &pushPlayer(GOPlayer *player);
    
public:
    static GOBall *create(GameManager &manager);
    GOPlayer *getScorer();

private:
    std::mutex _mutex;
    std::list<GOPlayer *> _players;
};

#endif /* GOBall_hpp */
