//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/worldobject.cc
// Implementacao da classe WorldObject.
//

#include "worldobject.h"
#include "../scenes/world.h"
#include "../utils/circleobject.h"

namespace sprite {

using namespace framework;
using namespace scene;
using namespace utils;

WorldObject::WorldObject()
    : //Sprite(NULL),
      bound_(NULL),
      status_(STATUS_ACTIVE),
      collision_type_(NO_COLLISION)
{}

WorldObject::WorldObject(WorldObject* obj) /*: Sprite(NULL) */{
    // vazio
}

WorldObject::~WorldObject() {
}

void WorldObject::Update(float dt) {
    World *world = ((World *)Engine::reference()->CurrentScene());

    Sprite::Update(dt);
    set_position(world->FromWorldCoordinates(world_position())); // Transforma coordenadas mundo -> coordenadas tela
    set_zindex(world->FromWorldLinearCoordinates(world_position()).y); // Seta zindex
}

bool WorldObject::IsColliding(WorldObject* obj) const {
    return bound_->Intersects(obj->bound());
}

void WorldObject::HandleCollision(WorldObject* obj) {
//     double dispatch
//     http://en.wikipedia.org/wiki/Double_dispatch
    obj->CollidesWith(this);
}

}  // namespace sprite
