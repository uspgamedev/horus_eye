#include "game/components/animation.h"

#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/time/timeaccumulator.h>

#include "game/components/graphic.h"
#include "game/components/animator.h"

using ugdk::math::Vector2D;

namespace component {

//int Animation::direction_mapping_[8] = {
//    /*[0] */ Animation_::RIGHT,
//    /*[1] */ Animation_::RIGHT | Animation_::UP,
//    /*[2] */ Animation_::UP,
//    /*[3] */ Animation_::UP | Animation_::LEFT,
//    /*[4] */ Animation_::LEFT,
//    /*[5] */ Animation_::LEFT | Animation_::DOWN,
//    /*[6] */ Animation_::DOWN,
//    /*[7] */ Animation_::DOWN | Animation_::RIGHT
//};
/*
direction_mapping_[0] = Animation_::RIGHT;
direction_mapping_[1] = Animation_::RIGHT | Animation_::UP;
direction_mapping_[2] = Animation_::UP;
direction_mapping_[3] = Animation_::UP | Animation_::LEFT;
direction_mapping_[4] = Animation_::LEFT;
direction_mapping_[5] = Animation_::LEFT | Animation_::DOWN;
direction_mapping_[6] = Animation_::DOWN;
direction_mapping_[7] = Animation_::DOWN | Animation_::RIGHT; 
*/

utils::AnimtionType Animation::default_animation_ = utils::IDLE;

Animation::Animation()
    :   owner_(nullptr)
    ,   current_animation_(utils::IDLE)
{}

Animation::Animation(utils::AnimtionType type, const Direction& dir)
    :   owner_(nullptr)
    ,   current_direction_(dir)
    ,   current_animation_(type)
{}

Animation::~Animation() {}

void Animation::Update(double dt) {}
    
void Animation::OnAdd(sprite::WorldObject* wobj) {
    owner_ = wobj;
    owner_->graphic()->animator()->AddObserver(this);
    updateGraphic();
}

void Animation::Tick() {
    executeCallback();
    current_animation_ = default_animation_;
    updateGraphic();
}

void Animation::ChangeDirection(const Direction& dir) { 
    current_direction_ = dir;
    updateGraphic();
}

bool Animation::ChangeAnimation(utils::AnimtionType type) {
    if(!CanInterrupt(type)) return false;
    if(current_animation_ != type)
        executeCallback();
    current_animation_ = type;
    return updateGraphic();
}

bool Animation::ChangeAnimation(utils::AnimtionType type, const Direction& dir) {
    current_direction_ = dir;
    return ChangeAnimation(type);
}
    
void Animation::FinishAnimation() {
    Tick();
}

bool Animation::CanInterrupt(utils::AnimtionType type) const {
    return current_animation_ < type;
}
    
bool Animation::IsAnimation(utils::AnimtionType type) const {
    return current_animation_ == type;
}

void Animation::executeCallback() {
    // If the animation we're interrupting has a callback on it's end, call it.
    if(animation_callbacks_[current_animation_])
        animation_callbacks_[current_animation_](owner_);
}
    
bool Animation::updateGraphic() {
    return owner_->graphic()->animator()->ChangeAnimation(current_animation_, current_direction_);
}

}  // namespace component
