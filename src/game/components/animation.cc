#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/action/animationset.h>
#include <ugdk/time/timeaccumulator.h>
#include "game/components/animation.h"
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

Animation::Animation(sprite::WorldObject* wobj, const std::string& spritesheet_tag,
                     utils::IsometricAnimationSet* iso_animation_set)
    :   Base("animation"),
        owner_(wobj),
        sprite_(new ugdk::graphic::Sprite(spritesheet_tag, iso_animation_set->animation_set())),
        isometric_animation_set_(iso_animation_set),
        current_animation_(utils::STANDING),
        has_queued_animation_(false),
        uninterrutible_(false) {
        
    wobj->graphic()->node()->set_drawable(sprite_);
    sprite_->AddObserverToAnimation(this);
}

Animation::~Animation() {}

void Animation::Update(double dt) {

}

void Animation::Tick() {
    if(animation_callbacks_[current_animation_])
        animation_callbacks_[current_animation_](owner_);
    uninterrutible_ = false;
    if(has_queued_animation_) {
        has_queued_animation_ = false;
        AddComponent(queued_animation_);
    }
}

void Animation::set_direction(const Direction& dir) { 
    current_direction_ = dir;
    if(!uninterrutible_) sprite_->SelectAnimation(isometric_animation_set_->Get(current_animation_, current_direction_));
}

void Animation::AddComponent(utils::AnimtionType type) { 
    if(current_animation_ != type && animation_callbacks_[current_animation_])
        animation_callbacks_[current_animation_](owner_);

    current_animation_ = type;
    if(!uninterrutible_) sprite_->SelectAnimation(isometric_animation_set_->Get(current_animation_, current_direction_));
}

void Animation::queue_animation(utils::AnimtionType type) {
    queued_animation_ = type;
    has_queued_animation_ = true;
}

}  // namespace component
