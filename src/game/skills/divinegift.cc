#include "game/skills/divinegift.h"

#include "game/resources/energy.h"
#include "game/resources/capacityblocks.h"
#include "game/components/caster.h"

namespace skills {

void DivineGift::Use(component::Caster* caster) { 
    caster->mana_blocks() -= block_cost_;
    super::Use(caster);
};

bool DivineGift::Available(const component::Caster* caster) const {
    double total_mana_cost = super::mana_cost() + caster->mana_blocks().ToMana(block_cost_);
    return caster->mana().Has(total_mana_cost) && caster->mana_blocks().Has(block_cost_);
}

bool DivineGift::IsValidUse(const component::Caster* caster) const { 
    return super::IsValidUse(caster);
}

} // skills
