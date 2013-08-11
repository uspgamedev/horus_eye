#include <sstream>
#include <cassert>

#include "game/utils/isometricanimationset.h"

#include <ugdk/system/engine.h>
#include <ugdk/resource/module.h>
#include <ugdk/structure/indexabletable.h>

using ugdk::math::Vector2D;
using component::Direction;

namespace utils {

std::map<Direction, std::string> DIRECTION_VALUES_CREATOR() {
    std::map<Direction, std::string> result_set;

    result_set[Direction()                           ] = "";
    result_set[Direction::Down()                     ] = "_DOWN";
    result_set[                    Direction::Left() ] = "_LEFT";
    result_set[                    Direction::Right()] = "_RIGHT";
    result_set[Direction::Up()                       ] = "_UP";
    result_set[Direction::Down() | Direction::Right()] = "_DOWN_RIGHT";
    result_set[Direction::Down() | Direction::Left() ] = "_DOWN_LEFT";
    result_set[Direction::Up()   | Direction::Right()] = "_UP_RIGHT";
    result_set[Direction::Up()   | Direction::Left() ] = "_UP_LEFT";
    
    return result_set;
}
std::map<Direction, std::string> DIRECTION_VALUES = DIRECTION_VALUES_CREATOR();

std::map<AnimtionType, std::string> ANIMATIONTYPE_NAMES_CREATOR() {
    std::map<AnimtionType, std::string> result_set;

    result_set[IDLE] = "STANDING";
    result_set[MOVEMENT] = "WALKING";
    result_set[ATTACK] = "ATTACKING";
    result_set[TAKING_HIT] = "TAKING_DAMAGE";
    result_set[SPAWNING] = "SPAWNING";
    result_set[DEATH] = "DYING";

    return result_set;
}
std::map<AnimtionType, std::string> ANIMATIONTYPE_NAMES = ANIMATIONTYPE_NAMES_CREATOR();


IsometricAnimationSet::IsometricAnimationSet(ugdk::action::SpriteAnimationTable* animation_set)
    :   animation_set_(animation_set) {
        
    for(auto names = ANIMATIONTYPE_NAMES.begin(); names != ANIMATIONTYPE_NAMES.end(); ++names) {
        for(auto dir = DIRECTION_VALUES.begin(); dir != DIRECTION_VALUES.end(); ++dir) {
            std::stringstream composed_name;
            composed_name << names->second<< dir->second;

            int index = animation_set->MakeIndex(composed_name.str());
            if(index != -1)
                animation_cache_[names->first][dir->first] = index;
        }
    }
}

IsometricAnimationSet::~IsometricAnimationSet() {}

int IsometricAnimationSet::Get(AnimtionType type, const component::Direction& dir) const {
    return getAnimationFromCache(animation_cache_[type], dir);
}
    
IsometricAnimationSet* IsometricAnimationSet::LoadFromFile(const std::string& name) {
    ugdk::action::SpriteAnimationTable* set = ugdk::resource::GetSpriteAnimationTableFromFile(name);
    if(set)
        return new IsometricAnimationSet(set);
    return NULL;
}

IsometricAnimationSet* IsometricAnimationSet::LoadFromResourceManager(const std::string& name) {
    return ugdk::resource::manager()->get_container<IsometricAnimationSet*>()->Load(name, name);
}
    
int IsometricAnimationSet::getAnimationFromCache(const AnimationDirectionCache& map, const component::Direction& dir) const {
    // Check if this animation has the expected direction
    auto expected_animation = map.find(dir);
    if(expected_animation != map.end())
        return expected_animation->second;

    // Try using the 'no-direction' direction for this animation then
    auto no_direction = map.find(Direction());
    if(no_direction != map.end())
        return no_direction->second;

    // We don't have this animation, return error code.
    return -1;
}

}  // namespace component
