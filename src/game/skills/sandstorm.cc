
#include "game/skills/sandstorm.h"
#include "game/utils/constants.h"

namespace skills {

using utils::Constants;

Sandstorm::Sandstorm(sprite::Hero* owner) 
  : CombatArt<usearguments::Aim>(NULL, 0.0f, owner->mana(), owner->aim()),
    actual_mana_cost_(mana_cost_ * Constants::SANDSTORM_MIN_REPEAT) {}

void Sandstorm::Use() {
    


}

} // namespace skills