#include <cmath>
#include <ugdk/math/vector2D.h>

#include "item.h"

#include "game/sprites/creatures/hero.h"
#include "game/utils/circleobject.h"
#include "game/utils/constants.h"

#define CENTER_X    Constants::POTION_SPRITE_CENTER_X
#define CENTER_Y    Constants::POTION_SPRITE_CENTER_Y
#define HEIGHT      Constants::POTION_HEIGHT

#define PI 3.141592654f
#define PERIOD (2.0f*PI/3.0f)

namespace sprite {

using namespace ugdk;
using namespace utils;

Item::Item(ugdk::Image* img) : total_time_((1.0f*rand()/RAND_MAX)*PERIOD) {
    Initialize(img);
    set_hotspot(Vector2D(CENTER_X, CENTER_Y + HEIGHT));
    this->bound_ = new CircleObject(0.15f);
    this->collision_type_ = STATIC;
    this->event_ = NULL;

    known_collisions_[GET_COLLISIONMASK(Hero)] = new Collisions::Use(this);
}

void Item::Update(float delta_t) {
	WorldObject::Update(delta_t);
    total_time_ += delta_t;
    if (total_time_ >= PERIOD) total_time_ -= PERIOD;
    set_hotspot(Vector2D(CENTER_X, CENTER_Y + HEIGHT + 10.0f*cos(3.0f*total_time_)));
}

COLLISION_IMPLEMENT(Item, Use, obj) {
    Hero *hero = (Hero*) obj;
    if (owner_->event_->Use(hero))
        owner_->status_ = WorldObject::STATUS_DEAD;
}

}
