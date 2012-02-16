#ifndef HORUSEYE_GAME_SKILLS_HEROMETEORWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROMETEORWEAPON_H_

#include "game/skills/divinegift.h"
#include "game/skills/usearguments.h"

namespace sprite {
    class Hero;
}

namespace skills {

class HeroMeteorWeapon : public DivineGift<usearguments::Aim> {
  public:
    HeroMeteorWeapon(sprite::Hero* owner);
    virtual void Use();
    virtual bool IsValidUse() const;
  private:
    typedef DivineGift<usearguments::Aim> super;
};

} // namespace skills

#endif /* HORUSEYE_GAME_SKILLS_HEROMETEORWEAPON_H_ */
