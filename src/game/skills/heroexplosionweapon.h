#ifndef HORUSEYE_GAME_SKILLS_HEROEXPLOSIONWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROEXPLOSIONWEAPON_H_

#include "game/components.h"
#include "game/skills/divinegift.h"
#include "game/skills/usearguments.h"

namespace skills {

class HeroExplosionWeapon : public DivineGift<usearguments::Aim> {
  public:
    HeroExplosionWeapon(component::Hero* owner);
    virtual void Use();
    virtual bool IsValidUse() const;

  private:
    typedef DivineGift<usearguments::Aim> super;

    const static double range_;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_ */
