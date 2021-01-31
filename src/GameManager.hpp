//
//  GameManager.hpp
//  GGJ2021
//
//  Created by Karim DRIDI on 31/01/2021.
//

#ifndef GameManager_hpp
#define GameManager_hpp

#include <box2d/box2d.h>
#include <string>

class GOPlayer;
class GOBall;
class GOGoal;

enum EntityCategory
{
    BOUNDARY = 0x0001,
    PLAYER = 0x0002,
    BALL = 0x0004,
    GOAL = 0x0008,
};

class GameManager : public b2ContactListener
{
public:
    GameManager();
    ~GameManager();

    void createGame();
    void destroyGame();
    std::string updateGame();

    GOPlayer *createPlayer();
    void destroyPlayer(GOPlayer *player);

    GOBall *createBall();
    void destroyBall(GOBall *body);

public:
    virtual void EndContact(b2Contact *contact);

public:
    b2World &world() { return *_world; }

private:
    friend class GameObject;

    bool _score = true;
    b2World *_world = nullptr;
    GOBall *_ball = nullptr;
    GOGoal *_rgoal = nullptr;
    GOGoal *_lgoal = nullptr;
    b2Vec2 const _dimensions{80.0f, 60.0f};
};

#endif /* GameManager_hpp */
