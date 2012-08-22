#include <sstream>

#include <ugdk/action/animationset.h>
#include "game/utils/isometricanimationset.h"

using ugdk::Vector2D;
using component::Direction;

typedef std::pair<int, const char*> DirectionValue;
static DirectionValue DIRECTION_VALUES[8] = {
    DirectionValue((Direction::Down()                     ).value(), "DOWN"),
    DirectionValue((                    Direction::Left() ).value(), "LEFT"),
    DirectionValue((                    Direction::Right()).value(), "RIGHT"),
    DirectionValue((Direction::Up()                       ).value(), "UP"),
    DirectionValue((Direction::Down() | Direction::Right()).value(), "DOWN_RIGHT"),
    DirectionValue((Direction::Down() | Direction::Left() ).value(), "DOWN_LEFT"),
    DirectionValue((Direction::Up()   | Direction::Right()).value(), "UP_RIGHT"),
    DirectionValue((Direction::Up()   | Direction::Left() ).value(), "UP_LEFT")

};

namespace utils {

IsometricAnimationSet::IsometricAnimationSet(ugdk::action::AnimationSet* animation_set)
    :   animation_set_(animation_set) {
        
    for(int i = 0; i < 16; i++)
        animation_index_[STANDING][i] = animation_index_[WALKING][i] = animation_index_[ATTACKING][i] = -1;

    for(int i = 0; i < 8; ++i) {
        const DirectionValue& it = DIRECTION_VALUES[i];
        std::stringstream standing, walking, attacking;
        standing << "STANDING_" << it.second;
        walking << "WALKING_" <<  it.second;
        attacking << "ATTACKING_" << it.second;
        animation_index_[STANDING][it.first] = animation_set->MakeIndex(standing.str());
        animation_index_[WALKING][it.first] = animation_set->MakeIndex(walking.str());
        animation_index_[ATTACKING][it.first] = animation_set->MakeIndex(attacking.str());
    }

    int dying = animation_set->MakeIndex("DYING");
    int taking_damage = animation_set->MakeIndex("TAKING_DAMAGE");
    for(int i = 0; i < 16; i++) {
        animation_index_[DYING][i] = dying;
        animation_index_[TAKING_HIT][i] = taking_damage;
    }
}

IsometricAnimationSet::~IsometricAnimationSet() {}

int IsometricAnimationSet::Get(AnimtionType type, const component::Direction& dir) {
    /*switch(type) {
    default:*/
        return animation_index_[type][dir.value()];
    //}
}

}  // namespace component
