#ifndef HORUSEYE_GAME_SPRITES_WEAPONS_WEAPON_H_
#define HORUSEYE_GAME_SPRITES_WEAPONS_WEAPON_H_
#include <ugdk/graphic/image.h>

namespace sprite {
class Creature;
class Weapon {
  public:
    Weapon(Creature* owner) { owner_ = owner; icon_ = NULL; }
    virtual ~Weapon() {}
	virtual float range() = 0;
	virtual void Attack() = 0;
	virtual bool Available() = 0;
        ugdk::Image* icon() { return icon_; }
          

  protected:
	Creature* owner_;
        ugdk::Image* icon_;
};

}//namespace

#endif /* HORUSEYE_GAME_SPRITES_WEAPONS_WEAPON_H_ */
