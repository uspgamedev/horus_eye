#ifndef HORUSEYE_GAME_SKILLS_HEROLIGHTWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROLIGHTWEAPON_H_

#include "game/skills/combatart.h"
#include "game/sprites/creatures/hero.h"
#include "game/utils/constants.h"
#include "game/skills/castarguments.h"

namespace sprite {
class Hero;
}

namespace skills {

class HeroLightWeapon : public CombatArt<castarguments::Aim> {
  public:
    HeroLightWeapon(sprite::Hero* owner);

    virtual float range() const { return utils::Constants::QUAKE_EXPLOSION_RANGE; } // TODO: change to another value
    virtual void Attack();

  private:
    sprite::Hero *hero_;
};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_HEROLIGHTWEAPON_H_ */
