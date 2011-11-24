#include <ugdk/graphic/videomanager.h>
#include <ugdk/base/engine.h>
#include <ugdk/action/animationset.h>
#include <pyramidworks/geometry/rect.h>

#include "button.h"

#include "game/utils/constants.h"
#include "game/utils/tile.h"
#include "game/scenes/world.h"

using namespace ugdk;
using namespace utils;
using namespace scene;

namespace sprite {

INITIALIZE_COLLIDABLE_NODE(Button, WorldObject);

COLLISION_DIRECT(Button*, PressCollision, obj) {
    data_->Press();
}

Button::Button(ugdk::Image* image, scene::World *world)
  : super(image),
    reactive_time_(new ugdk::TimeAccumulator(1000)),
    world_(world) {

    pressed_ = false;

    INITIALIZE_COLLISION;
    SET_COLLISIONCLASS(Button);
    SET_COLLISIONSHAPE(new pyramidworks::geometry::Rect(0.75f, 0.75f));

    ADD_COLLISIONLOGIC(Hero, new PressCollision(this));
    ADD_COLLISIONLOGIC(Block, new PressCollision(this));
}

void Button::Update(float delta_t) {
    super::Update(delta_t);
    if(pressed_ && reactive_time_->Expired()) {
        DePress();
    }
    SetDefaultFrame((int)(!pressed_) * 2);
}

void Button::Press() {
    pressed_ = true;
    reactive_time_->Restart();
    world_->set_button_pressed(pressed_);
}

void Button::DePress() {
    pressed_ = false;
    world_->set_button_pressed(pressed_);
}

}



