//
//  GameManager.cpp
//  GGJ2021
//
//  Created by Karim DRIDI on 31/01/2021.
//

#include "GameManager.hpp"
#include "WebSocketSerializer.hpp"
#include "GOPlayer.hpp"

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
    return GOPlayer::create(*this);
}

void GameManager::destroyPlayer(GOPlayer *player)
{
    delete player;
}

void GameManager::BeginContact(b2Contact *contact)
{
    b2Body *ba = contact->GetFixtureA()->GetBody();
    b2Body *bb = contact->GetFixtureB()->GetBody();

    b2Body *other = nullptr;
    if (ba == _ball)
        other = bb;
    if (bb == _ball)
        other = ba;

    if (other != nullptr)
    {
        if (other == _lgoal)
        {
            _score = true;
        }
        if (other == _rgoal)
        {
            _score = true;
        }
    }
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

        b2PolygonShape shape;
        shape.SetAsBox(dim.x / 2, dim.y / 2);

        b2BodyDef bd;
        bd.type = b2_staticBody;
        bd.position.Set(0 - (_dimensions.x - 4.0 * dim.x) / 2.0, 0.0f);

        b2FixtureDef fd;
        fd.shape = &shape;
        fd.filter.categoryBits = GOAL;
        fd.filter.maskBits = RTEAM | BALL;
        fd.density = 2.0f;

        _lgoal = _world->CreateBody(&bd);
        _lgoal->CreateFixture(&fd);
    }
    // goal right
    {
        b2Vec2 dim{width, 5.0f * width};

        b2PolygonShape shape;
        shape.SetAsBox(dim.x / 2, dim.y / 2);

        b2BodyDef bd;
        bd.type = b2_staticBody;
        bd.position.Set(0 + (_dimensions.x - 4.0 * dim.x) / 2.0, 0.0f);

        b2FixtureDef fd;
        fd.shape = &shape;
        fd.filter.categoryBits = GOAL;
        fd.filter.maskBits = LTEAM | BALL;
        fd.density = 2.0f;

        _rgoal = _world->CreateBody(&bd);
        _rgoal->CreateFixture(&fd);
    }
}

void GameManager::destroyGame()
{
}

std::string GameManager::updateGame()
{
    WebSocketSerializer serializer{};

    _world->Step(.030f, 10, 10);

    if (_score == true)
    {
        if (_ball != nullptr)
            destroyBall(_ball);
        _ball = createBall();
        _score = false;
    }

    return serializer.serialize(*_world);
}

b2Body *GameManager::createBall()
{
    b2CircleShape shape;
    shape.m_radius = width;

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.filter.categoryBits = BALL;
    fd.filter.maskBits = LTEAM | RTEAM | BOUNDARY | GOAL;
    fd.density = 1.0f;
    fd.restitution = 1.0f;

    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(0.0f, 0.0f);

    b2Body *body = _world->CreateBody(&bd);
    body->CreateFixture(&fd);

    return body;
}

void GameManager::destroyBall(b2Body *body)
{
    _world->DestroyBody(body);
}
