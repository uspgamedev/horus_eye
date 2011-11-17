#ifndef HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_
#define HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_

#include "game/resources/simpleresource.h"
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
	virtual float range() = 0;
	virtual void Attack() = 0;
	virtual bool Available() const = 0;
    // end

    virtual void Use() { Attack(); }
    virtual bool IsValidUse() const { return Available(); }

  protected:
    DivineGift(ugdk::Image* icon,
               float cost,
               resource::SimpleResource& caster_mana,
               resource::CountableResource& caster_blocks,
               const CastArgument& cast_argument
              )
      : CombatArt<CastArgument>(icon,cost,caster_mana,cast_argument),
        caster_blocks_(caster_blocks) {}

    resource::CountableResource& caster_blocks_;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_ */
