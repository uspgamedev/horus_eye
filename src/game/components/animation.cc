#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/action/animationset.h>
#include <ugdk/time/timeaccumulator.h>
#include "game/components/animation.h"

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

Animation::Animation(Graphic* graphic, ugdk::graphic::Spritesheet *spritesheet, utils::IsometricAnimationSet* iso_animation_set)
    :   graphic_(graphic),
        sprite_(new ugdk::graphic::Sprite(spritesheet, iso_animation_set->animation_set())),
        isometric_animation_set_(iso_animation_set) {
        
    graphic->node()->set_drawable(sprite_);
    sprite_->AddObserverToAnimation(this);
}

Animation::~Animation() {}

void Animation::Update(double dt) {

}

void Animation::Tick() {
    sprite_->SelectAnimation(isometric_animation_set_->Get(current_animation_, current_direction_));
}

void Animation::set_direction(const Direction& dir) { 
    current_direction_ = dir;
    sprite_->SelectAnimation(isometric_animation_set_->Get(current_animation_, current_direction_));
}

void Animation::select_animation(utils::AnimtionType types) { 
    current_animation_ = types;
    sprite_->SelectAnimation(isometric_animation_set_->Get(current_animation_, current_direction_));
}

}  // namespace component
