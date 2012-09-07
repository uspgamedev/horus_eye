#include "game/skills/combatart.h"

#include "game/resources/energy.h"
#include "game/components/caster.h"

namespace skills {

void CombatArt::Use(component::Caster* caster) {
    caster->mana() -= mana_cost_;
    if(use_) use_(caster);
}

bool CombatArt::Available(const component::Caster* caster) const { 
    return caster->mana().Has(mana_cost_);
}

bool CombatArt::IsValidUse(const component::Caster* caster) const { 
    return true;
}

} // namespace skills
