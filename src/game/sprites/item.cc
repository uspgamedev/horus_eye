//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/lifepotion.cc
// lifepotion.cc da classe LifePotion
//

#include <math.h>
#include "item.h"
#include "../utils/circleobject.h"
#include "../utils/constants.h"
#include "../../framework/vector2D.h"

#define CENTER_X    Constants::POTION_SPRITE_CENTER_X
#define CENTER_Y    Constants::POTION_SPRITE_CENTER_Y
#define HEIGHT      Constants::POTION_HEIGHT

namespace sprite {

using namespace framework;
using namespace utils;

Item::Item(framework::Image* img) {
    Initialize(img);
    set_hotspot(Vector2D(CENTER_X, CENTER_Y + HEIGHT));
    this->bound_ = new CircleObject(0.15f);
    this->collision_type_ = STATIC;
    this->event_ = NULL;
}

void Item::Update(float delta_t) {
	WorldObject::Update(delta_t);
    total_time_ += delta_t;
    set_hotspot(Vector2D(CENTER_X, CENTER_Y + HEIGHT + 10.0f*cos(3.0f*total_time_)));
}

void Item::CollidesWith(Hero *obj) {
    if (event_->Use(obj))
        this->status_ = WorldObject::STATUS_DEAD;
}

void Item::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

}
