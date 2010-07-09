//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/creature.cc
// Implementacao da classe Creature.
//

#include "creature.h"
#include "worldobject.h"
#include "../utils/circleobject.h"
#include "../utils/rectobject.h"

#include <cmath>

using namespace framework;
using namespace utils;

#define PI acos(-1)

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

    const CircleObject *circle = (const CircleObject*)bound_;
    const RectObject *rect = (const RectObject*)obj->bound();
    Vector2D    line(rect->width(), rect->height()),
                circ_pos = circle->position(),
                rect_pos = rect->position();
    float tg = line.y/line.x;


    int i = circ_pos.y < rect_pos.y + tg*(circ_pos.x - rect_pos.x),
        j = circ_pos.y < rect_pos.y - tg*(circ_pos.x - rect_pos.x);

    if ((i+j)%2) walking_direction_.x = 0;
    else         walking_direction_.y = 0;

    walking_direction_ = Vector2D::Normalized(walking_direction_);

}

void Creature::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

int Creature::GetAttackingAnimationIndex(double angle) {
    int degreeAngle = (int)((angle / PI) * 360);
    degreeAngle += 45;
    int animationIndex = degreeAngle / 90;
    return animationIndex % 8;
}

double Creature::GetAttackingAngle(Vector2D targetDirection) {
    Vector2D versor = Vector2D::Normalized(targetDirection);
    double radianAngle = acos(versor.x);
    if (versor.y > 0) {
        radianAngle = 2*PI - radianAngle;
    }
    return radianAngle;
}

void Creature::SelectSpriteAnimation(Animation *animation, Vector2D frame_size) {
    this->image()->set_frame_size(frame_size);
    this->SelectAnimation(animation);
}

}  // namespace sprite
