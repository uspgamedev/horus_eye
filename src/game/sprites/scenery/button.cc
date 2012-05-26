#include <ugdk/graphic/videomanager.h>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/spritesheet/flexiblespritesheet.h>
#include <ugdk/graphic/node.h>
#include <ugdk/action/animationset.h>
#include <pyramidworks/geometry/rect.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>

#include "button.h"

#include "game/scenes/world.h"
#include "game/utils/constants.h"

using namespace ugdk;
using namespace utils;
using namespace scene;

#define SECONDS_TO_MILISECONDS(sec) (int)((sec) * 1000)

namespace sprite {

COLLISION_DIRECT(Button*, PressCollision, obj) {
    data_->Press();
}

Button::Button(ugdk::graphic::FlexibleSpritesheet* image, scene::World *world, double active_time)
    :   world_(world), sprite_(new ugdk::graphic::Sprite(image)) {

    image->set_hotspot(Vector2D(Constants::FLOOR_HOTSPOT_X, Constants::FLOOR_HOTSPOT_Y));
    node()->set_drawable(sprite_);
    if(active_time > 0.0) {
        reactive_time_ = new ugdk::time::TimeAccumulator(SECONDS_TO_MILISECONDS(active_time));
    } else {
        reactive_time_ = NULL;
    }

    world->num_button_not_pressed() += 1;

    pressed_ = false;

    INITIALIZE_COLLISION;
    SET_COLLISIONCLASS(Button);
    SET_COLLISIONSHAPE(new pyramidworks::geometry::Rect(0.75, 0.75));

    ADD_COLLISIONLOGIC(Hero, new PressCollision(this));
    ADD_COLLISIONLOGIC(Block, new PressCollision(this));
}

void Button::Update(double delta_t) {
    super::Update(delta_t);
    if(pressed_ && reactive_time_ && reactive_time_->Expired()) {
        DePress();
    }
    sprite_->SetDefaultFrame((int)(!pressed_) * 2); 
}

void Button::Die() {
    if(pressed_)
        pressed_ = false;
    else
        world_->num_button_not_pressed() -= 1;
}

void Button::Press() {
    if(!pressed_) {
        world_->num_button_not_pressed() -= 1;
        pressed_ = true;
    }
    if(reactive_time_) reactive_time_->Restart();
}

void Button::DePress() {
    if(pressed_) {
        pressed_ = false;
        world_->num_button_not_pressed() += 1;
    }
}

void Button::set_world_position(const ugdk::Vector2D& pos) {
    super::set_world_position(pos);
    node()->set_zindex(-FLT_MAX); // floor must be below everything
}


}



