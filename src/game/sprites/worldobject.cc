//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/worldobject.cc
// Implementacao da classe WorldObject.
//

#include "worldobject.h"

namespace sprite {

using namespace framework;

WorldObject::WorldObject()
    : Sprite(NULL),
      world_position_( framework::Vector2D(0.0f, 0.0f) ),
      collision_radius_(0.0f),
      status_(STATUS_ACTIVE)
{}

WorldObject::WorldObject(WorldObject* obj) : Sprite(NULL) {
    // vazio
}

WorldObject::~WorldObject() {
}

bool WorldObject::IsColliding(WorldObject* obj) const {
    framework::Vector2D distance = obj->world_position() - world_position();
    return distance.length() <= obj->collision_radius() + collision_radius();
}

void WorldObject::CollidesWithG(WorldObject* obj) {
    // double dispatch
    // http://en.wikipedia.org/wiki/Double_dispatch
    obj->CollidesWith(this);
}

}  // namespace sprite
