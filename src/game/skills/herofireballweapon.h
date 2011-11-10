#ifndef HORUSEYE_GAME_SKILLS_HEROFIREBALLWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROFIREBALLWEAPON_H_

#include "game/skills/combatart.h"

namespace sprite {
class Hero;
}

namespace skills {

class HeroFireballWeapon : public CombatArt {
  public:
    HeroFireballWeapon(sprite::Hero* owner);

    virtual float range(){ return 0.0f; }
    virtual void Attack();
    virtual bool Available() const;

  private:

    sprite::Hero *hero_;
    int cost_;

};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_ */
