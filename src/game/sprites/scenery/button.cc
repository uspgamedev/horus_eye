#include <ugdk/graphic/videomanager.h>
#include <ugdk/base/engine.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/action/animationset.h>
#include <pyramidworks/geometry/rect.h>

#include "button.h"

#include "game/utils/constants.h"
#include "game/utils/tile.h"
#include "game/scenes/world.h"

using namespace ugdk;
using namespace utils;
using namespace scene;

#define SECONDS_TO_MILISECONDS(sec) (int)((sec) * 1000)

namespace sprite {

COLLISION_DIRECT(Button*, PressCollision, obj) {
    data_->Press();
}

Button::Button(ugdk::FlexibleSpritesheet* image, scene::World *world, float active_time)
  : super(image),
    world_(world) {

    if(active_time > 0.0f) {
        reactive_time_ = new ugdk::TimeAccumulator(SECONDS_TO_MILISECONDS(active_time));
    } else {
        reactive_time_ = NULL;
    }

    world->num_button_not_pressed() += 1;

    pressed_ = false;

    INITIALIZE_COLLISION;
    SET_COLLISIONCLASS(Button);
    SET_COLLISIONSHAPE(new pyramidworks::geometry::Rect(0.75f, 0.75f));

    ADD_COLLISIONLOGIC(Hero, new PressCollision(this));
    ADD_COLLISIONLOGIC(Block, new PressCollision(this));
}

void Button::Update(float delta_t) {
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

}



