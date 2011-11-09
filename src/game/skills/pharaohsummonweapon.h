#ifndef HORUSEYE_GAME_SPRITES_WEAPONS_PHARAOHSUMMONWEAPON_H_
#define HORUSEYE_GAME_SPRITES_WEAPONS_PHARAOHSUMMONWEAPON_H_

#include "weapon.h"
#include "game/utils/constants.h"
#include "game/sprites/creatures/creature.h"

namespace sprite {

class PharaohSummonWeapon: public sprite::Weapon {
  public:
    PharaohSummonWeapon(Creature* owner) : Weapon(owner) {}
    virtual ~PharaohSummonWeapon() {}

    virtual float range(){ return utils::Constants::PHARAOH_SUMMON_RANGE; }
    virtual void Attack();
    virtual bool Available() {
        return owner_->mana() >= utils::Constants::PHARAOH_SUMMON_MANA_COST;
    }
};

}

#endif /* HORUSEYE_GAME_SPRITES_WEAPONS_PHARAOHSUMMONWEAPON_H_ */
