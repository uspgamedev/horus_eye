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

Creature::Creature() : WorldObject(),
                     last_stable_position_(0, 0) { this->collision_type_ = MOVEABLE; }
                     
void Creature::Move(Vector2D direction, float delta_t) {
    Vector2D position(this->world_position().x, this->world_position().y);
    last_stable_position_ = position;
    position = position + direction * (this->speed_ * delta_t);
    set_world_position(position);
}

void Creature::CollidesWith(Wall * obj) {

    set_world_position(last_stable_position_);

}  // namespace sprite

void Creature::SelectSpriteAnimation(Animation *animation, Vector2D frame_size) {
    this->image()->set_frame_size(frame_size);
    this->SelectAnimation(animation);
}

}
