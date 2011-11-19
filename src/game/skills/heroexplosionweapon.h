#ifndef HORUSEYE_GAME_SKILLS_HEROEXPLOSIONWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROEXPLOSIONWEAPON_H_

#include "game/skills/divinegift.h"
#include "game/sprites/creatures/hero.h"
#include "game/utils/constants.h"
#include "game/skills/usearguments.h"

namespace sprite {
class Hero;
}

namespace skills {

class HeroExplosionWeapon : public DivineGift<usearguments::Aim> {
  public:
    HeroExplosionWeapon(sprite::Hero* owner);
    virtual void Use();
    virtual bool IsValidUse() const;

  private:
    typedef DivineGift<usearguments::Aim> super;

    const static float range_;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_ */
