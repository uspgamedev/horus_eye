#ifndef HORUSEYE_GAME_SKILLS_CASTTYPES_AIMEDCAST_H_
#define HORUSEYE_GAME_SKILLS_CASTTYPES_AIMEDCAST_H_

#include "game/skills/casttypes/casttype.h"
#include "game/sprites/casttypes/ressources.h"

namespace skills {
namespace casttypes {

class AimedCast : public CastType<Aim> {

  public:
    AimedCast(const Aim& aim) : aim_(aim) {}
    const Aim& UsageData() { return aim_; }

  private:
    const Aim& aim_;

};

} // casttypes
} // skills

#endif