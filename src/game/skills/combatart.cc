#include "game/skills/combatart.h"

#include "game/resources/energy.h"
#include "game/components/caster.h"

namespace skills {

void CombatArt::Use(component::Caster* caster) {
    caster->mana() -= mana_cost_;
    super::Use(caster);
}

bool CombatArt::Available(const component::Caster* caster) const { 
    return caster->mana().Has(mana_cost_);
}

bool CombatArt::IsValidUse(const component::Caster* caster) const {
    return 
    ((range_ < 0.0) || ((caster->aim().destination_ - caster->aim().origin_).Length() < range_)) &&
        super::IsValidUse(caster);
}

} // namespace skills
