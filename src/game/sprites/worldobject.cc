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
    World *world = ((World *)Engine::reference()->CurrentScene());
    set_position(world->FromWorldCoordinates(world_position_));
    // TODO: Setar z-index corretamente

    set_zindex(world->FromWorldLinearCoordinates(world_position_).y);

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
