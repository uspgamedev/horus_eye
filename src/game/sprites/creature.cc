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

#include <cmath>
#include <iostream>

using namespace framework;
using namespace utils;

#define PI acos(-1)

namespace sprite {

Creature::Creature() : WorldObject(),
                     last_stable_position_(0, 0),
                     hit_duration_(NULL) { this->collision_type_ = MOVEABLE; }

Creature::~Creature() {

    if (hit_duration_) delete hit_duration_;

    SelectAnimation(NULL);

    for (int i = 1; i < 9; i <<= 1){
        delete *standing_animations_[i];
        free(standing_animations_[i]);
        delete *walking_animations_[i];
        free(walking_animations_[i]);
        if (i >= 4) for (int j = 1; j < 3; j <<= 1) {
            delete *standing_animations_[i|j];
            free(standing_animations_[i|j]);
            delete *walking_animations_[i|j];
            free(walking_animations_[i|j]);
        }
    }

    for (int i = 0; i < 8; i++) {
        delete attacking_animations_[i];
    }
    delete dying_animation_;

}

void Creature::InitializeWalkingAnimations(){
    for (int i = 0; i < 16; i++) {
        walking_animations_[i] = (Animation **) malloc (sizeof (Animation *));
        *walking_animations_[i] = NULL;
    }
    *walking_animations_[Animation_::DOWN] = new Animation(10, 4, 14, 24, 34, 44, -1);
    *walking_animations_[Animation_::LEFT] = new Animation(10, 7, 17, 27, 37, 47, -1);
    *walking_animations_[Animation_::RIGHT] = new Animation(10, 2, 12, 22, 32, 42, -1);
    *walking_animations_[Animation_::UP] = new Animation(10, 0, 10, 20, 30, 40, -1);
    *walking_animations_[Animation_::DOWN | Animation_::RIGHT] = new Animation(10, 3, 13, 23, 33, 43, -1);
    *walking_animations_[Animation_::DOWN | Animation_::LEFT] = new Animation(10, 6, 16, 26, 36, 46, -1);
    *walking_animations_[Animation_::UP | Animation_::RIGHT] = new Animation(10, 1, 11, 21, 31, 41, -1);
    *walking_animations_[Animation_::UP | Animation_::LEFT] = new Animation(10, 8, 18, 28, 38, 48, -1);
    for (int i = 0; i < 16; i++) {
        if (*walking_animations_[i] == NULL) {
            free(walking_animations_[i]);
            walking_animations_[i] = &last_standing_animation_;
        }
    }
}
void Creature::InitializeStandingAnimations(){
    for (int i = 0; i < 16; i++) {
        standing_animations_[i] = (Animation **) malloc (sizeof (Animation *));
        *standing_animations_[i] = NULL;
    }
    *standing_animations_[Animation_::DOWN] = new Animation(0, 4, -1);
    *standing_animations_[Animation_::LEFT] = new Animation(0, 7, -1);
    *standing_animations_[Animation_::RIGHT] = new Animation(0, 2, -1);
    *standing_animations_[Animation_::UP] = new Animation(0, 0, -1);
    *standing_animations_[Animation_::DOWN | Animation_::RIGHT] = new Animation(0, 3, -1);
    *standing_animations_[Animation_::DOWN | Animation_::LEFT] = new Animation(0, 6, -1);
    *standing_animations_[Animation_::UP | Animation_::RIGHT] = new Animation(0, 1, -1);
    *standing_animations_[Animation_::UP | Animation_::LEFT] = new Animation(0, 8, -1);
    for (int i = 0; i < 16; i++) {
        if (*standing_animations_[i] == NULL) {
            free(standing_animations_[i]);
            standing_animations_[i] = &last_standing_animation_;
        }
    }
}

void Creature::Move(Vector2D direction, float delta_t) {
    Vector2D position(this->world_position().x, this->world_position().y);
    last_stable_position_ = position;
    position = position + direction * (this->speed_ * delta_t);
    set_world_position(position);
}

void Creature::CollideWithRect(const RectObject *rect) {

    set_world_position(last_stable_position_);

    const CircleObject *circle = (const CircleObject*)bound_;
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

void Creature::CollidesWith(Wall * obj) {
    const RectObject *rect = (const RectObject*)obj->bound();
    CollideWithRect(rect);
}

void Creature::CollidesWith(Door * obj) {
    const RectObject *rect = (const RectObject*)obj->bound();
    CollideWithRect(rect);
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
