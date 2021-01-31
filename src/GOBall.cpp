//
//  GOBall.cpp
//  GGJ2021
//
//  Created by Karim DRIDI on 31/01/2021.
//

#include "GOBall.hpp"

static const float width = 2.0f;

GOBall *GOBall::create(GameManager &manager) {
    b2CircleShape shape;
    shape.m_radius = width;

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.filter.categoryBits = BALL;
    fd.filter.maskBits = PLAYER | BOUNDARY | GOAL;
    fd.density = 1.0f;
    fd.restitution = 1.0f;

    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(0.0f, 0.0f);

    b2Body *body = manager.world().CreateBody(&bd);
    body->CreateFixture(&fd);

    return new GOBall(manager, *body);
}

GOBall &GOBall::pushPlayer(GOPlayer *player) {
    const std::lock_guard<std::mutex> lock(_mutex);
    
    while (_players.size() > 5)
        _players.pop_back();
        
    _players.push_front(player);
    
    return (*this);
}
