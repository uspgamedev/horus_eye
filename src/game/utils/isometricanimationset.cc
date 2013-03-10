#include <sstream>
#include <cassert>

#include "game/utils/isometricanimationset.h"

#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/util/indexabletable.h>

using ugdk::math::Vector2D;
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

IsometricAnimationSet::IsometricAnimationSet(ugdk::action::SpriteAnimationTable* animation_set)
    :   animation_set_(animation_set) {
        
    for(int i = 0; i < 16; i++)
        animation_index_[IDLE][i] = animation_index_[MOVEMENT][i] = animation_index_[ATTACK][i] = -1;

    for(int i = 0; i < 8; ++i) {
        const DirectionValue& it = DIRECTION_VALUES[i];
        std::stringstream standing, walking, attacking;
        standing << "STANDING_" << it.second;
        walking << "WALKING_" <<  it.second;
        attacking << "ATTACKING_" << it.second;
        animation_index_[IDLE][it.first] = animation_set->MakeIndex(standing.str());
        animation_index_[MOVEMENT][it.first] = animation_set->MakeIndex(walking.str());
        animation_index_[ATTACK][it.first] = animation_set->MakeIndex(attacking.str());
    }

    int dying = animation_set->MakeIndex("DYING");
    int taking_damage = animation_set->MakeIndex("TAKING_DAMAGE");

    for(int i = 0; i < 16; i++) {
        animation_index_[DEATH][i] = dying;
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

IsometricAnimationSet* IsometricAnimationSet::LoadFromFile(const std::string& name) {
    ugdk::action::SpriteAnimationTable* set = ugdk::base::ResourceManager::GetSpriteAnimationTableFromFile(name);
    if(set)
        return new IsometricAnimationSet(set);
    return NULL;
}

IsometricAnimationSet* IsometricAnimationSet::LoadFromResourceManager(const std::string& name) {
    return RESOURCE_MANAGER()->get_container<IsometricAnimationSet*>().Load(name, name);
}

}  // namespace component
