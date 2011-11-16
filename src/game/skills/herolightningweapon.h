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

    virtual float range(){ return 0.0f; }
    virtual void Attack();
    virtual bool Available() const;

  private:

    sprite::Hero *hero_;
    int cost_;

};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_ */
