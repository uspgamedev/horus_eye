#ifndef HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_
#define HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_

#include "game/resources/energy.h"
#include "game/resources/countableresource.h"

#include "game/skills/combatart.h"

namespace ugdk {
class Image;
}

namespace skills {

template<class CastArgument_T>
class DivineGift : public CombatArt<CastArgument_T> {
  public:
    virtual ~DivineGift() {}
    typedef CastArgument_T CastArgument;

    // We need to remove these.
	virtual float range() const = 0;
	virtual void Attack() = 0;
    virtual bool Available() const { return caster_mana_.Has(mana_cost_) && caster_blocks_.Has(block_cost_); }
    /* virtual bool Available() const {
        float total_mana_cost = mana_cost_ + resources::ToMana(block_cost_);
        return caster_mana_.Has(total_mana_cost) && caster_blocks_.Has(block_cost_);
    } */
    // end

    virtual void Use() { Attack(); }
    virtual bool IsValidUse() const { return Available(); }

  protected:
    DivineGift(ugdk::Image* icon,
               float mana_cost,
               int block_cost,
               resource::Energy& caster_mana,
               resource::CountableResource& caster_blocks,
               const CastArgument& cast_argument
              )
      : CombatArt<CastArgument>(icon,mana_cost,caster_mana,cast_argument),
        block_cost_(block_cost),
        caster_blocks_(caster_blocks) {}

    resource::CountableResource& caster_blocks_;
    const int block_cost_;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_ */
