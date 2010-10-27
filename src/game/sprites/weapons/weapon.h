#ifndef HORUSEYE_GAME_SPRITES_WEAPONS_WEAPON_H_
#define HORUSEYE_GAME_SPRITES_WEAPONS_WEAPON_H_
#include "../../../framework/image.h"

namespace sprite {
class Creature;
class Weapon {
  public:
    Weapon(Creature* owner) { owner_ = owner; icon_ = NULL; }
	virtual float range() = 0;
	virtual void Attack() = 0;
	virtual bool Available() = 0;
        framework::Image* icon() { return icon_; }
          

  protected:
	Creature* owner_;
        framework::Image* icon_;
};

}//namespace

#endif /* HORUSEYE_GAME_SPRITES_WEAPONS_WEAPON_H_ */
