//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/creature.cc
// Implementacao da classe Creature.
//

#include "creature.h"
#include "worldobject.h"

#include <cmath>

using namespace framework;

namespace sprite {

Creature::Creature() : WorldObject() { }

void Creature::Move(Vector2D direction, float delta_t) {
    Vector2D position(this->world_position().x, this->world_position().y);
    position = position + direction * (this->speed_ * delta_t);
    set_world_position(position);
}

void Creature::CollidesWith(Wall * obj) {
    Vector2D distance = this->world_position() - obj->world_position();
    float curDist = distance.length();
    float angle = atan2(distance.y, distance.x);
    this->set_world_position(
        obj->world_position() +
        Vector2D(cos(angle), sin(angle)) *
        ((this->collision_radius() + obj->collision_radius()) / curDist));
}

}  // namespace sprite
