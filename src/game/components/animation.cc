#include "game/components/animation.h"

#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/action/animationset.h>
#include <ugdk/time/timeaccumulator.h>

#include "game/components/graphic.h"

using ugdk::Vector2D;

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

Animation::Animation(sprite::WorldObject* wobj, const std::string& spritesheet_tag,
                     utils::IsometricAnimationSet* iso_animation_set)
    :   owner_(wobj),
        sprite_(new ugdk::graphic::Sprite(spritesheet_tag, iso_animation_set->animation_set())),
        isometric_animation_set_(iso_animation_set),
        current_animation_(utils::IDLE) {
        
    wobj->graphic()->node()->set_drawable(sprite_);
    sprite_->AddObserverToAnimation(this);
}

Animation::~Animation() {}

void Animation::Update(double dt) {}

void Animation::Tick() {
    // If the animation we're interrupting has a callback on it's end, call it.
    if(animation_callbacks_[current_animation_])
        animation_callbacks_[current_animation_](owner_);
    set_current_animation(default_animation_);
}

void Animation::ChangeDirection(const Direction& dir) { 
    current_direction_ = dir;
    int index = isometric_animation_set_->Get(current_animation_, current_direction_);
    if(index > -1) sprite_->SelectAnimation(index);
}

bool Animation::ChangeAnimation(utils::AnimtionType type) {
    if(!CanInterrupt(type)) return false;
    Tick();
    set_current_animation(type);
    return true;
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
    
void Animation::set_current_animation(utils::AnimtionType type) {
    int index = isometric_animation_set_->Get(type, current_direction_);
    if(index > -1) sprite_->SelectAnimation(index);
    current_animation_ = type;
}

}  // namespace component
