//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/creature.cc
// Implementacao da classe Creature.
//

#include "creature.h"

namespace sprite {

void Creature::Move(framework::Vector2D direction, float delta_t) {
    // TODO: implementar essa funcao
}

void Creature::CollidesWith(Wall * obj);
    framework::Vector2D distance = this.world_position() - obj.world_position();
    float curDist = sqrt(framework::Vector2D::InnerProduct(distance, distance));
    float angle = atan2(distance.y, distance.x);
    this.set_world_position(
        obj.world_position() + new framework::Vector2D(cos(angle), sin(angle)));
}

}  // namespace sprite
