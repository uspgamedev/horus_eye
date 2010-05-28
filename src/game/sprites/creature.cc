//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/creature.cc
// Implementacao da classe Creature.
//

#include "creature.h"

using namespace framework;

namespace sprite {

void Creature::Move(Vector2D direction, float delta_t) {
    // TODO: implementar essa funcao
}

void Creature::CollidesWith(Wall * obj);
    Vector2D distance = this->world_position() - obj->world_position();
    float curDist = sqrt(Vector2D::InnerProduct(distance, distance));
    float angle = atan2(distance.y, distance.x);
    this->set_world_position(
        obj->world_position() +
        new Vector2D(cos(angle), sin(angle)) *
        ((this->collision_radius() + obj->collision_radius()) / curDist )  );
}

}  // namespace sprite
