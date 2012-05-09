#include <ugdk/action/animationset.h>
#include "game/utils/isometricanimationset.h"

using ugdk::Vector2D;
using component::Direction;

namespace utils {

IsometricAnimationSet::IsometricAnimationSet(ugdk::action::AnimationSet* animation_set)
    :   animation_set_(animation_set) {
        
    for(int i = 0; i < 16; i++)
        animation_index_[STANDING][i] = animation_index_[WALKING][i] = animation_index_[ATTACKING][i] = -1;

    animation_index_[STANDING][(Direction::Down()                     ).value()] = animation_set->MakeIndex("STANDING_DOWN");
    animation_index_[STANDING][(                    Direction::Left() ).value()] = animation_set->MakeIndex("STANDING_LEFT");
    animation_index_[STANDING][(                    Direction::Right()).value()] = animation_set->MakeIndex("STANDING_RIGHT");
    animation_index_[STANDING][(Direction::Up()                       ).value()] = animation_set->MakeIndex("STANDING_UP");
    animation_index_[STANDING][(Direction::Down() | Direction::Right()).value()] = animation_set->MakeIndex("STANDING_DOWN_RIGHT");
    animation_index_[STANDING][(Direction::Down() | Direction::Left() ).value()] = animation_set->MakeIndex("STANDING_DOWN_LEFT");
    animation_index_[STANDING][(Direction::Up()   | Direction::Right()).value()] = animation_set->MakeIndex("STANDING_UP_RIGHT");
    animation_index_[STANDING][(Direction::Up()   | Direction::Left() ).value()] = animation_set->MakeIndex("STANDING_UP_LEFT");

    animation_index_[WALKING][(Direction::Down()                     ).value()] = animation_set->MakeIndex("WALKING_DOWN");
    animation_index_[WALKING][(Direction::Up()                       ).value()] = animation_set->MakeIndex("WALKING_UP");
    animation_index_[WALKING][(                    Direction::Left() ).value()] = animation_set->MakeIndex("WALKING_LEFT");
    animation_index_[WALKING][(                    Direction::Right()).value()] = animation_set->MakeIndex("WALKING_RIGHT");
    animation_index_[WALKING][(Direction::Down() | Direction::Right()).value()] = animation_set->MakeIndex("WALKING_DOWN_RIGHT");
    animation_index_[WALKING][(Direction::Down() | Direction::Left() ).value()] = animation_set->MakeIndex("WALKING_DOWN_LEFT");
    animation_index_[WALKING][(Direction::Up()   | Direction::Right()).value()] = animation_set->MakeIndex("WALKING_UP_RIGHT");
    animation_index_[WALKING][(Direction::Up()   | Direction::Left() ).value()] = animation_set->MakeIndex("WALKING_UP_LEFT");

    animation_index_[ATTACKING][6] = animation_set->MakeIndex("ATTACKING_DOWN");
    animation_index_[ATTACKING][4] = animation_set->MakeIndex("ATTACKING_LEFT");
    animation_index_[ATTACKING][0] = animation_set->MakeIndex("ATTACKING_RIGHT");
    animation_index_[ATTACKING][2] = animation_set->MakeIndex("ATTACKING_UP");
    animation_index_[ATTACKING][7] = animation_set->MakeIndex("ATTACKING_DOWN_RIGHT");
    animation_index_[ATTACKING][5] = animation_set->MakeIndex("ATTACKING_DOWN_LEFT");
    animation_index_[ATTACKING][1] = animation_set->MakeIndex("ATTACKING_UP_RIGHT");
    animation_index_[ATTACKING][3] = animation_set->MakeIndex("ATTACKING_UP_LEFT");
}

IsometricAnimationSet::~IsometricAnimationSet() {}

}  // namespace component
