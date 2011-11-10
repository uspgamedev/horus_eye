#ifndef HORUSEYE_GAME_SKILLS_HEROEXPLOSIONWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROEXPLOSIONWEAPON_H_
#include "game/skills/combatart.h"
#include "game/sprites/creatures/hero.h"
#include "game/utils/constants.h"

namespace sprite {
class Hero;
}

namespace skills {

class HeroExplosionWeapon : public CombatArt {
  public:
    HeroExplosionWeapon(sprite::Hero* owner);

    virtual float range() { return utils::Constants::QUAKE_EXPLOSION_RANGE; }
    virtual void Attack();
    virtual bool Available() const;

  private:
    sprite::Hero *hero_;
    int cost_;
};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_ */
