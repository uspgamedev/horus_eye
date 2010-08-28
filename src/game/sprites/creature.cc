//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/creature.cc
// Implementacao da classe Creature.
//

#include "creature.h"
#include "worldobject.h"
#include "mummy.h"
#include "hero.h"
#include "../utils/circleobject.h"

#include <cmath>
#include <iostream>

using namespace framework;
using namespace utils;

#define PI acos(-1.0f)

namespace sprite {

Creature::Creature() : 	WorldObject() { 
	last_stable_position_ = Vector2D(0,0);
	hit_duration_ = NULL;
	waiting_animation_ = false;
	this->collision_type_ = MOVEABLE; 
}

Creature::~Creature() {

    if (hit_duration_) delete hit_duration_;

    // Para evitar double free.
    SelectAnimation(NULL);

    // Remove todas as animaÃ§Ãµes.
    for (int i = 0; i < 8; i++) {
        delete *standing_animations_[direction_mapping_[i]];
        free(standing_animations_[direction_mapping_[i]]);
        delete *walking_animations_[direction_mapping_[i]];
        free(walking_animations_[direction_mapping_[i]]);
    }

    for (int i = 0; i < 8; i++) {
        delete attacking_animations_[i];
    }
    delete dying_animation_;

}

void Creature::TakeDamage(int life_points) {
    life_ -= life_points;
    if(life_ <= 0) {
        if (status_ == WorldObject::STATUS_ACTIVE) {
            this->SelectAnimation(dying_animation_);
            this->status_ = WorldObject::STATUS_DYING;
            this->collision_type_ = WorldObject::NO_COLLISION;
        }
    }
}

void Creature::InitializeAttackingAnimations() {
    attacking_animations_[6] = new Animation(10, 44, 54, 64, 74, -1);
    attacking_animations_[4] = new Animation(10, 47, 57, 67, 77, -1);
    attacking_animations_[0] = new Animation(10, 42, 52, 62, 72, -1);
    attacking_animations_[2] = new Animation(10, 40, 50, 60, 70, -1);
    attacking_animations_[7] = new Animation(10, 43, 53, 63, 73, -1);
    attacking_animations_[5] = new Animation(10, 46, 56, 66, 76, -1);
    attacking_animations_[1] = new Animation(10, 41, 51, 61, 71, -1);
    attacking_animations_[3] = new Animation(10, 48, 58, 68, 78, -1);

    for (int i = 0; i < 8; i++) {
        attacking_animations_[i]->AddObserver(this);
    }
}

void Creature::InitializeWalkingAnimations() {
    for (int i = 0; i < 16; i++) {
        walking_animations_[i] = (Animation **) malloc (sizeof (Animation *));
        *walking_animations_[i] = NULL;
    }
    *walking_animations_[Animation_::DOWN] = new Animation(10, 4, 14, 24, 34, -1);
    *walking_animations_[Animation_::LEFT] = new Animation(10, 7, 17, 27, 37, -1);
    *walking_animations_[Animation_::RIGHT] = new Animation(10, 2, 12, 22, 32, -1);
    *walking_animations_[Animation_::UP] = new Animation(10, 0, 10, 20, 30, -1);
    *walking_animations_[Animation_::DOWN | Animation_::RIGHT] = new Animation(10, 3, 13, 23, 33, -1);
    *walking_animations_[Animation_::DOWN | Animation_::LEFT] = new Animation(10, 6, 16, 26, 36, -1);
    *walking_animations_[Animation_::UP | Animation_::RIGHT] = new Animation(10, 1, 11, 21, 31, -1);
    *walking_animations_[Animation_::UP | Animation_::LEFT] = new Animation(10, 8, 18, 28, 38, -1);
    for (int i = 0; i < 16; i++) {
        if (*walking_animations_[i] == NULL) {
            free(walking_animations_[i]);
            walking_animations_[i] = &last_standing_animation_;
        }
    }
}

void Creature::InitializeStandingAnimations() {
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
    float       left = rect_pos.x - line.x/2,
                bottom = rect_pos.y - line.y/2,
                right = left + line.x,
                top = bottom + line.y;

    if (circ_pos.y < top && circ_pos.y > bottom)
        walking_direction_.x = 0;
    else if (circ_pos.x < right && circ_pos.x > left)
        walking_direction_.y = 0;
    else {
        Vector2D point(left, bottom);
        if (circ_pos.x > right) point.x  = right;
        if (circ_pos.y > top)   point.y = top;
        Vector2D dir = circ_pos - point,
                 tg_dir(-dir.y, dir.x);
        if (Vector2D::InnerProduct(walking_direction_, tg_dir) > 0)
            walking_direction_ = tg_dir;
        else
            walking_direction_ = Vector2D()-tg_dir;
    }

    walking_direction_ = Vector2D::Normalized(walking_direction_);

}

void Creature::Tick() {
    if (status_ == WorldObject::STATUS_DYING) {
            status_ = WorldObject::STATUS_DEAD;
    }
	waiting_animation_ = false;
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

}  // namespace sprite
