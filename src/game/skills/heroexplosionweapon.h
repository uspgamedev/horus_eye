#ifndef HORUSEYE_GAME_SKILLS_HEROEXPLOSIONWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROEXPLOSIONWEAPON_H_

#include "game/skills/combatart.h"
#include "game/sprites/creatures/hero.h"
#include "game/utils/constants.h"
#include "game/skills/castarguments.h"

namespace sprite {
class Hero;
}

namespace skills {

class HeroExplosionWeapon : public CombatArt<castarguments::Aim> {
  public:
    HeroExplosionWeapon(sprite::Hero* owner);

    virtual float range() const { return utils::Constants::QUAKE_EXPLOSION_RANGE; }
    virtual void Attack();
    virtual bool Available() const;
};

} // skills

#endif /* HORUSEYE_GAME_SKILLS_HEROBASEWEAPON_H_ */
