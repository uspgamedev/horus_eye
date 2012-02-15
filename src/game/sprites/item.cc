#include <cmath>
#include <ugdk/math/vector2D.h>
#include <ugdk/graphic.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <pyramidworks/geometry/circle.h>

#include "item.h"

#include "game/sprites/creatures/hero.h"
#include "game/utils/constants.h"

#define CENTER_X    Constants::POTION_SPRITE_CENTER_X
#define CENTER_Y    Constants::POTION_SPRITE_CENTER_Y
#define HEIGHT      Constants::POTION_HEIGHT

#define PI 3.141592654
#define PERIOD (2.0*PI/3.0)

namespace sprite {

using namespace ugdk;
using namespace utils;

COLLISION_DIRECT(Item*, UseCollision, obj) {
    Hero *hero = (Hero*) obj;
    if (data_->event_->Use(hero))
        data_->Die();
}

Item::Item(ugdk::graphic::FlexibleSpritesheet* img) : total_time_((1.0*rand()/RAND_MAX)*PERIOD) {
    img->set_hotspot(Vector2D(CENTER_X, CENTER_Y + HEIGHT));

    this->node()->set_drawable(new ugdk::graphic::Sprite(img));
    this->event_ = NULL;

    INITIALIZE_COLLISION;
    SET_COLLISIONCLASS(Item);
    SET_COLLISIONSHAPE(new pyramidworks::geometry::Circle(0.15));
    ADD_COLLISIONLOGIC(Hero, new UseCollision(this));
}

void Item::Update(double delta_t) {
	WorldObject::Update(delta_t);
    total_time_ += delta_t;
    if (total_time_ >= PERIOD) total_time_ -= PERIOD;
	this->node()->drawable()->set_hotspot(Vector2D(0.0, 10.0*cos(3.0*total_time_)));
}

}
