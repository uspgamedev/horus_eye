
#ifndef HORUSEYE_GAME_SKILLS_MUMMYWEAPON_H_
#define HORUSEYE_GAME_SKILLS_MUMMYWEAPON_H_
#include "game/skills/combatart.h"

namespace sprite {
class Creature;
}

namespace skills {

class MummyWeapon : public CombatArt {
  public:
    MummyWeapon(sprite::Creature* owner, int damage = 1) : CombatArt(owner), damage_(damage) {}

    virtual float range(){ return 1.0f; }
    virtual void Attack();
    virtual bool Available() const { return true; }
  private:
    int damage_;
};

}//namespace

#endif /* HORUSEYE_GAME_SKILLS_MUMMYWEAPON_H_ */
