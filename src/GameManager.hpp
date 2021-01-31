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

enum EntityCategory
{
    BOUNDARY = 0x0001,
    LTEAM = 0x0002,
    RTEAM = 0x0004,
    BALL = 0x0008,
    GOAL = 0x0010,
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

    b2Body *createBall();
    void destroyBall(b2Body *body);

public:
    virtual void BeginContact(b2Contact *contact);

public:
    b2World &world() { return *_world; }

private:
    friend class GameObject;

    bool _score = true;
    b2World *_world = nullptr;
    b2Body *_ball = nullptr;
    b2Body *_rgoal = nullptr;
    b2Body *_lgoal = nullptr;
    b2Vec2 const _dimensions{80.0f, 60.0f};
};

#endif /* GameManager_hpp */
