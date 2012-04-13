#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/action/animationset.h>
#include <ugdk/time/timeaccumulator.h>
#include "game/components/animation.h"

using ugdk::Vector2D;

namespace component {

int Animation::direction_mapping_[8] = {
    /*[0] */ Animation_::RIGHT,
    /*[1] */ Animation_::RIGHT | Animation_::UP,
    /*[2] */ Animation_::UP,
    /*[3] */ Animation_::UP | Animation_::LEFT,
    /*[4] */ Animation_::LEFT,
    /*[5] */ Animation_::LEFT | Animation_::DOWN,
    /*[6] */ Animation_::DOWN,
    /*[7] */ Animation_::DOWN | Animation_::RIGHT
};
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

Animation::Animation(Graphic* graphic, ugdk::graphic::Spritesheet *spritesheet, ugdk::AnimationSet* animation_set)
    :   graphic_(graphic),
        sprite_(new ugdk::graphic::Sprite(spritesheet, animation_set)),
        animation_set_(animation_set) {
        
    graphic->node()->set_drawable(sprite_);
    sprite_->AddObserverToAnimation(this);
    
    for(int i = 0; i < 16; i++)
        animation_index_[STANDING][i] = animation_index_[WALKING][i] = animation_index_[ATTACKING][i] = -1;

    animation_index_[STANDING][Animation_::DOWN                    ] = animation_set->MakeIndex("STANDING_DOWN");
    animation_index_[STANDING][                   Animation_::LEFT ] = animation_set->MakeIndex("STANDING_LEFT");
    animation_index_[STANDING][                   Animation_::RIGHT] = animation_set->MakeIndex("STANDING_RIGHT");
    animation_index_[STANDING][Animation_::UP                      ] = animation_set->MakeIndex("STANDING_UP");
    animation_index_[STANDING][Animation_::DOWN | Animation_::RIGHT] = animation_set->MakeIndex("STANDING_DOWN_RIGHT");
    animation_index_[STANDING][Animation_::DOWN | Animation_::LEFT ] = animation_set->MakeIndex("STANDING_DOWN_LEFT");
    animation_index_[STANDING][Animation_::UP   | Animation_::RIGHT] = animation_set->MakeIndex("STANDING_UP_RIGHT");
    animation_index_[STANDING][Animation_::UP   | Animation_::LEFT ] = animation_set->MakeIndex("STANDING_UP_LEFT");

    animation_index_[WALKING][Animation_::DOWN                    ] = animation_set->MakeIndex("WALKING_DOWN");
    animation_index_[WALKING][Animation_::UP                      ] = animation_set->MakeIndex("WALKING_UP");
    animation_index_[WALKING][                   Animation_::LEFT ] = animation_set->MakeIndex("WALKING_LEFT");
    animation_index_[WALKING][                   Animation_::RIGHT] = animation_set->MakeIndex("WALKING_RIGHT");
    animation_index_[WALKING][Animation_::DOWN | Animation_::RIGHT] = animation_set->MakeIndex("WALKING_DOWN_RIGHT");
    animation_index_[WALKING][Animation_::DOWN  | Animation_::LEFT ] = animation_set->MakeIndex("WALKING_DOWN_LEFT");
    animation_index_[WALKING][Animation_::UP   | Animation_::RIGHT] = animation_set->MakeIndex("WALKING_UP_RIGHT");
    animation_index_[WALKING][Animation_::UP   | Animation_::LEFT ] = animation_set->MakeIndex("WALKING_UP_LEFT");

    animation_index_[ATTACKING][6] = animation_set->MakeIndex("ATTACKING_DOWN");
    animation_index_[ATTACKING][4] = animation_set->MakeIndex("ATTACKING_LEFT");
    animation_index_[ATTACKING][0] = animation_set->MakeIndex("ATTACKING_RIGHT");
    animation_index_[ATTACKING][2] = animation_set->MakeIndex("ATTACKING_UP");
    animation_index_[ATTACKING][7] = animation_set->MakeIndex("ATTACKING_DOWN_RIGHT");
    animation_index_[ATTACKING][5] = animation_set->MakeIndex("ATTACKING_DOWN_LEFT");
    animation_index_[ATTACKING][1] = animation_set->MakeIndex("ATTACKING_UP_RIGHT");
    animation_index_[ATTACKING][3] = animation_set->MakeIndex("ATTACKING_UP_LEFT");
}

Animation::~Animation() {}

}  // namespace component
