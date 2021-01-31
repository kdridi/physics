//
//  WebSocketSerializer.cpp
//  GGJ2021
//
//  Created by Karim DRIDI on 30/01/2021.
//

#include "WebSocketSerializer.hpp"

WebSocketSerializer::WebSocketSerializer()
{
}

WebSocketSerializer::~WebSocketSerializer()
{
}

void WebSocketSerializer::serialize(ggj2021::b2World *w, b2World &world, std::string const& scorer)
{
    this->_world = w;
    this->SetFlags(b2Draw::e_shapeBit);
    world.SetDebugDraw(this);
    world.DebugDraw();
}

void WebSocketSerializer::DrawPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    ggj2021::b2Polygon *polygon = _world->add_polygons();
    polygon->set_solid(false);
    for (int32 i = 0; i < vertexCount; i++)
    {
        ggj2021::b2Vec2 *v = polygon->add_vertices();
        v->set_x(vertices[i].x);
        v->set_y(vertices[i].y);
    }
    ggj2021::b2Color *polygon_color = polygon->mutable_color();
    polygon_color->set_r(color.r);
    polygon_color->set_g(color.g);
    polygon_color->set_b(color.b);
    polygon_color->set_a(color.a);
}

void WebSocketSerializer::DrawSolidPolygon(const b2Vec2 *vertices, int32 vertexCount, const b2Color &color)
{
    ggj2021::b2Polygon *polygon = _world->add_polygons();
    polygon->set_solid(true);
    for (int32 i = 0; i < vertexCount; i++)
    {
        ggj2021::b2Vec2 *v = polygon->add_vertices();
        v->set_x(vertices[i].x);
        v->set_y(vertices[i].y);
    }
    ggj2021::b2Color *polygon_color = polygon->mutable_color();
    polygon_color->set_r(color.r);
    polygon_color->set_g(color.g);
    polygon_color->set_b(color.b);
    polygon_color->set_a(color.a);
}

void WebSocketSerializer::DrawCircle(const b2Vec2 &center, float radius, const b2Color &color)
{
    ggj2021::b2Circle *circle = _world->add_circles();
    circle->set_solid(false);
    circle->set_radius(radius);
    ggj2021::b2Vec2 *circle_center = circle->mutable_center();
    circle_center->set_x(center.x);
    circle_center->set_y(center.y);
    ggj2021::b2Color *circle_color = circle->mutable_color();
    circle_color->set_r(color.r);
    circle_color->set_g(color.g);
    circle_color->set_b(color.b);
    circle_color->set_a(color.a);
}

void WebSocketSerializer::DrawSolidCircle(const b2Vec2 &center, float radius, const b2Vec2 &axis, const b2Color &color)
{
    ggj2021::b2Circle *circle = _world->add_circles();
    circle->set_solid(true);
    circle->set_radius(radius);
    ggj2021::b2Vec2 *circle_center = circle->mutable_center();
    circle_center->set_x(center.x);
    circle_center->set_y(center.y);
    ggj2021::b2Color *circle_color = circle->mutable_color();
    circle_color->set_r(color.r);
    circle_color->set_g(color.g);
    circle_color->set_b(color.b);
    circle_color->set_a(color.a);
}

void WebSocketSerializer::DrawSegment(const b2Vec2 &p1, const b2Vec2 &p2, const b2Color &color)
{
    ggj2021::b2Segment *segment = _world->add_segments();
    ggj2021::b2Vec2 *segment_p1 = segment->mutable_p1();
    segment_p1->set_x(p1.x);
    segment_p1->set_y(p1.y);
    ggj2021::b2Vec2 *segment_p2 = segment->mutable_p2();
    segment_p2->set_x(p2.x);
    segment_p2->set_y(p2.y);
    ggj2021::b2Color *segment_color = segment->mutable_color();
    segment_color->set_r(color.r);
    segment_color->set_g(color.g);
    segment_color->set_b(color.b);
    segment_color->set_a(color.a);
}

void WebSocketSerializer::DrawTransform(const b2Transform &xf)
{
    ggj2021::b2Transform *transform = _world->add_transforms();
    transform->set_angle(xf.q.GetAngle());
    ggj2021::b2Vec2 *transform_position = transform->mutable_position();
    transform_position->set_x(xf.p.x);
    transform_position->set_y(xf.p.y);
}

void WebSocketSerializer::DrawPoint(const b2Vec2 &p, float size, const b2Color &color)
{
    ggj2021::b2Point *circle = _world->add_points();
    circle->set_size(size);
    ggj2021::b2Vec2 *circle_p = circle->mutable_position();
    circle_p->set_x(p.x);
    circle_p->set_y(p.y);
    ggj2021::b2Color *circle_color = circle->mutable_color();
    circle_color->set_r(color.r);
    circle_color->set_g(color.g);
    circle_color->set_b(color.b);
    circle_color->set_a(color.a);
}
