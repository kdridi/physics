//
//  WebSocketSerializer.hpp
//  GGJ2021
//
//  Created by Karim DRIDI on 30/01/2021.
//

#ifndef WebSocketSerializer_hpp
#define WebSocketSerializer_hpp

#include <box2d/box2d.h>

#include "proto/box2d.pb.h"

class WebSocketSerializer : public b2Draw
{
public:
    WebSocketSerializer();
    virtual ~WebSocketSerializer();

    void serialize(ggj2021::b2World *w, b2World &world, std::string const& scorer);

public:
    virtual void DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color);
    virtual void DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color);
    virtual void DrawCircle(const b2Vec2 &center, float radius, const b2Color &color);
    virtual void DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color);
    virtual void DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color);
    virtual void DrawTransform(const b2Transform &xf);
    virtual void DrawPoint(const b2Vec2 &p, float size, const b2Color &color);

private:
    ggj2021::b2World *_world;
};

#endif /* WebSocketSerializer_hpp */
