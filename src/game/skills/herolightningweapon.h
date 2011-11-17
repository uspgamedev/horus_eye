#ifndef HORUSEYE_GAME_SKILLS_HEROLIGHTNINGWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROLIGHTNINGWEAPON_H_

#include "game/skills/combatart.h"
#include "game/skills/castarguments.h"

namespace sprite {
class Hero;
}

namespace skills {

class HeroLightningWeapon : public CombatArt<castarguments::Aim> {
  public:
    HeroLightningWeapon(sprite::Hero* owner);

    virtual float range() const { return 0.0f; }
    virtual void Attack();

  private:
    sprite::Hero *hero_;

};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_ */
