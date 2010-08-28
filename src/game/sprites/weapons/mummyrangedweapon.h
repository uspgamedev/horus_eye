#ifndef HORUSEYE_GAME_SPRITES_MUMMYRANGEDWEAPON_H_
#define HORUSEYE_GAME_SPRITES_MUMMYRANGEDWEAPON_H_
#include "weapon.h"

namespace sprite {

class MummyRangedWeapon : public Weapon {
	public:
	MummyRangedWeapon(Creature* owner) : Weapon(owner) {}

	virtual float range(){ return 5.0f; }
	virtual void Attack();
	virtual bool Available() { return true; }

};

}//namespace

#endif /* MUMMYRANGEDWEAPON_H_ */
