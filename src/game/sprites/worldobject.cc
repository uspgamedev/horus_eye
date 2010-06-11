//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/worldobject.cc
// Implementacao da classe WorldObject.
//

#include "worldobject.h"
#include "../scenes/world.h"

namespace sprite {

using namespace framework;

WorldObject::WorldObject() { }

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
