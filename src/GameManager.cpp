//
//  GameManager.cpp
//  GGJ2021
//
//  Created by Karim DRIDI on 31/01/2021.
//

#include "GameManager.hpp"
#include "WebSocketSerializer.hpp"
#include "GOPlayer.hpp"
#include "GOBall.hpp"
#include "GOGoal.hpp"

GameManager::GameManager()
{
    b2Vec2 gravity;
    gravity.Set(0.0f, 0.0f);
    _world = new b2World{gravity};
    _world->SetContactListener(this);
}

GameManager::~GameManager()
{
    if (_world != nullptr)
    {
        delete _world;
    }
}

GOPlayer *GameManager::createPlayer()
{
    return GOPlayer::create(*this, true);
}

void GameManager::destroyPlayer(GOPlayer *player)
{
    delete player;
}

void GameManager::EndContact(b2Contact *contact)
{
    b2Body *ba = contact->GetFixtureA()->GetBody();
    b2Body *bb = contact->GetFixtureB()->GetBody();
    
    if (GOBall::as(ba) != nullptr && GOPlayer::as(bb) != nullptr)
        GOBall::as(ba)->pushPlayer(GOPlayer::as(bb));
    
    if (GOBall::as(bb) != nullptr && GOPlayer::as(ba) != nullptr)
        GOBall::as(bb)->pushPlayer(GOPlayer::as(ba));
    
    if (GOBall::as(ba) != nullptr && GOGoal::as(bb) != nullptr)
        _score = true;
    if (GOBall::as(bb) != nullptr && GOGoal::as(ba) != nullptr)
        _score = true;
    
}

static const float width = 2.0f;

void GameManager::createGame()
{
#pragma mark spawn the field
    // border bottom
    {
        b2Vec2 dim{_dimensions.x, width};

        b2PolygonShape shape;
        shape.SetAsBox(dim.x / 2, dim.y / 2);

        b2BodyDef bd;
        bd.type = b2_staticBody;
        bd.position.Set(0.0f, 0 + (_dimensions.y - dim.y) / 2.0);

        b2FixtureDef fd;
        fd.isSensor = false;
        fd.shape = &shape;
        fd.filter.categoryBits = BOUNDARY;
        fd.density = 2.0f;

        b2Body *body = _world->CreateBody(&bd);
        body->CreateFixture(&fd);
    }
    // border bottom
    {
        b2Vec2 dim{_dimensions.x, width};

        b2PolygonShape shape;
        shape.SetAsBox(dim.x / 2, dim.y / 2);

        b2BodyDef bd;
        bd.type = b2_staticBody;
        bd.position.Set(0.0f, 0 - (_dimensions.y - dim.y) / 2.0);

        b2FixtureDef fd;
        fd.isSensor = false;
        fd.shape = &shape;
        fd.filter.categoryBits = BOUNDARY;
        fd.density = 2.0f;

        b2Body *body = _world->CreateBody(&bd);
        body->CreateFixture(&fd);
    }
    // border left
    {
        b2Vec2 dim{width, _dimensions.y - 2.0f * width};

        b2PolygonShape shape;
        shape.SetAsBox(dim.x / 2, dim.y / 2);

        b2BodyDef bd;
        bd.type = b2_staticBody;
        bd.position.Set(0 - (_dimensions.x - dim.x) / 2.0, 0.0f);

        b2FixtureDef fd;
        fd.isSensor = false;
        fd.shape = &shape;
        fd.filter.categoryBits = BOUNDARY;
        fd.density = 2.0f;

        b2Body *body = _world->CreateBody(&bd);
        body->CreateFixture(&fd);
    }
    // border right
    {
        b2Vec2 dim{width, _dimensions.y - 2.0f * width};

        b2PolygonShape shape;
        shape.SetAsBox(dim.x / 2, dim.y / 2);

        b2BodyDef bd;
        bd.type = b2_staticBody;
        bd.position.Set(0 + (_dimensions.x - dim.x) / 2.0, 0.0f);

        b2FixtureDef fd;
        fd.isSensor = false;
        fd.shape = &shape;
        fd.filter.categoryBits = BOUNDARY;
        fd.density = 2.0f;

        b2Body *body = _world->CreateBody(&bd);
        body->CreateFixture(&fd);
    }
#pragma mark spawn the goals
    // goal left
    {
        b2Vec2 dim{width, 5.0f * width};
        _lgoal = GOGoal::create(*this, 0 - (_dimensions.x - 4.0 * dim.x) / 2.0, 0.0f, dim.x / 2, dim.y / 2);
    }
    // goal right
    {
        b2Vec2 dim{width, 5.0f * width};
        _rgoal = GOGoal::create(*this, 0 + (_dimensions.x - 4.0 * dim.x) / 2.0, 0.0f, dim.x / 2, dim.y / 2);
    }
}

void GameManager::destroyGame()
{
}

std::string GameManager::updateGame()
{
    WebSocketSerializer serializer{};

    _world->Step(.030f, 10, 10);

    std::string scorer = "";
    if (_score == true)
    {
        if (_ball != nullptr) {
            auto player = _ball->getScorer();
            if (player != nullptr)
                scorer = player->name;
            destroyBall(_ball);
        }
        _ball = createBall();
        _score = false;
    }

    return serializer.serialize(*_world, scorer);
}

GOBall *GameManager::createBall()
{
    return GOBall::create(*this);
}

void GameManager::destroyBall(GOBall *ball)
{
    delete ball;
}
