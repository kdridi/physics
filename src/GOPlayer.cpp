//
//  GOPlayer.cpp
//  GGJ2021
//
//  Created by Karim DRIDI on 31/01/2021.
//

#include "GOPlayer.hpp"

GOPlayer *GOPlayer::create(GameManager &manager, bool left)
{
    b2PolygonShape shape;
    shape.SetAsBox(1.0, 1.0f);

    float px = 100.0 * (2.0 * ((double)rand() / (RAND_MAX)) - 1.0);
    px += px > 0 ? 0.0 + 100.0 : 0.0 - 100.0f;

    float py = 100.0 * (2.0 * ((double)rand() / (RAND_MAX)) - 1.0);
    py += py > 0 ? 0.0 + 100.0 : 0.0 - 100.0f;

    b2BodyDef bd;
    bd.type = b2_dynamicBody;
    bd.position.Set(px / 10.0, py / 10.0);

    b2FixtureDef fd;
    fd.shape = &shape;
    fd.filter.categoryBits = PLAYER;
    //        fd.filter.maskBits = LTEAM | RTEAM;
    fd.density = 10.0f;
    fd.restitution = 0.1f;

    b2Body *body = manager.world().CreateBody(&bd);
    body->CreateFixture(&fd);
    body->SetBullet(true);

    return new GOPlayer(manager, *body);
}

GOPlayer &GOPlayer::move(float x, float y)
{
    b2Vec2 force{x, y};
    b2Vec2 point = _body.GetPosition();
    _body.ApplyLinearImpulseToCenter(force, true);
    return *this;
}
