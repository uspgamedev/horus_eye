//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/worldobject.cc
// Implementacao da classe WorldObject.
//

#include "worldobject.h"
#include "../scenes/world.h"

#define SQRT_3 1.7320508075688772935274463415059

namespace sprite {

using namespace framework;
using namespace scene;

WorldObject::WorldObject()
    : //Sprite(NULL),
      world_position_( framework::Vector2D(0.0f, 0.0f) ),
      collision_radius_(0.0f),
      status_(STATUS_ACTIVE)
{}

WorldObject::WorldObject(WorldObject* obj) /*: Sprite(NULL) */{
    // vazio
}

WorldObject::~WorldObject() {
}

void WorldObject::Update(float dt) {

    Sprite::Update(dt);

    float       x = world_position_.x,
                y = world_position_.y;
    Vector2D    transformed;

    transformed.x = SQRT_3*(x - y)/2.0;
    transformed.y = -(x + y)/2.0;

    transformed = transformed*41 -
            ((World*)(Engine::reference()->CurrentScene()))->camera_position();

    set_position(transformed);

}

bool WorldObject::IsColliding(WorldObject* obj) const {
    framework::Vector2D distance = obj->world_position() - world_position();
    return distance.length() <= obj->collision_radius() + collision_radius();
}

void WorldObject::HandleCollision(WorldObject* obj) {
    // double dispatch
    // http://en.wikipedia.org/wiki/Double_dispatch
    obj->CollidesWith(this);
}

}  // namespace sprite
