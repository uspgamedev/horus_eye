//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/lifepotion.cc
// lifepotion.cc da classe LifePotion
//

#include <math.h>
#include "potion.h"
#include "../../framework/engine.h"
#include "../scenes/world.h"
#include "../utils/circleobject.h"
#include "../utils/constants.h"

#define CENTER_X    Constants::POTION_SPRITE_CENTER_X
#define CENTER_Y    Constants::POTION_SPRITE_CENTER_Y
#define HEIGHT      Constants::POTION_HEIGHT
#define POTION_SPRITE_WIDTH    Constants::POTION_SPRITE_WIDTH
#define POTION_SPRITE_HEIGHT   Constants::POTION_SPRITE_HEIGHT
#define POTION_RECOVER_LIFE   Constants::POTION_RECOVER_LIFE

namespace sprite {

Potion::Potion(framework::Image* img) {
    Initialize(img);
    recover_life_ = recover_mana_ = 0;
    total_time_ = 0;
    set_hotspot(Vector2D(CENTER_X, CENTER_Y + HEIGHT));
    this->bound_ = new CircleObject(0.15f);
    this->collision_type_ = STATIC;
}

void Potion::Update(float delta_t) {
	WorldObject::Update(delta_t);
    total_time_ += delta_t;
    set_hotspot(Vector2D(CENTER_X, CENTER_Y + HEIGHT + 10.0f*cos(3.0f*total_time_)));
}

Potion::~Potion () {
}

void Potion::CollidesWith(Hero *obj) {
    if ((recover_life_ != 0 && obj->life() < obj->max_life() )
         || (recover_mana_ != 0 && obj->mana() < obj->max_mana() ) )
        this->status_ = WorldObject::STATUS_DEAD;
}

void Potion::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

}