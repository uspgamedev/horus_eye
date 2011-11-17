#ifndef HORUSEYE_GAME_SKILLS_HEROMETEORWEAPON_H_
#define HORUSEYE_GAME_SKILLS_HEROMETEORWEAPON_H_

#include "game/skills/combatart.h"
#include "game/sprites/creatures/hero.h"
#include "game/utils/constants.h"
#include "game/skills/castarguments.h"

namespace skills {

class HeroMeteorWeapon : public CombatArt<castarguments::Aim> {
  public:
    HeroMeteorWeapon(sprite::Hero* owner);

    virtual float range() { return utils::Constants::QUAKE_EXPLOSION_RANGE; } // TODO: change to another value
    virtual void Attack();
    virtual bool Available() const;

  private:
    sprite::Hero *hero_;
    int cost_;
};

} // namespace

#endif /* HORUSEYE_GAME_SKILLS_HEROMETEORWEAPON_H_ */
