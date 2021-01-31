//
//  GOGoal.cpp
//  GGJ2021
//
//  Created by Karim DRIDI on 31/01/2021.
//

#include "GOGoal.hpp"

static const float width = 2.0f;

GOGoal *GOGoal::create(GameManager &manager, float x, float y, float w, float h) {
    b2PolygonShape shape;
    shape.SetAsBox(w, h);

    b2BodyDef bd;
    bd.type = b2_staticBody;
    bd.position.Set(x, y);

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.filter.categoryBits = GOAL;
    fd.filter.maskBits = PLAYER | BALL;
    fd.density = 2.0f;

    b2Body *body = manager.world().CreateBody(&bd);
    body->CreateFixture(&fd);
    
    return new GOGoal(manager, *body);
    
}
