#ifndef HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_
#define HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_

#include "game/resources/simpleresource.h"
#include "game/resources/blockresource.h"

#include "game/skills/skill.h"

namespace skills {

template<class CastArgument_T>
class DivineGift : public CombatArt<CastArgument_T> {
  public:
    virtual ~DivineGift() {}
    typedef CastArgument_T CastArgument;

  protected:

    CombatArt(ugdk::Image* icon,
              float cost,
              resource::SimpleResource& caster_mana,
              resource::CountableResource& caster_blocks
              const CastArgument& cast_argument)
      : ArgSkill<CastArgument>(icon, cast_argument),
        cost_(cost),
        caster_mana_(caster_mana),
        caster_blocks_(caster_blocks) {}

    resource::CountableResource& caster_blocks_;

};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_DIVINEGIFT_H_ */
