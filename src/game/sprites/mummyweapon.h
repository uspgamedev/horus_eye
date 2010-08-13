
#ifndef HORUSEYE_GAME_SPRITES_MUMMYWEAPON_H_
#define HORUSEYE_GAME_SPRITES_MUMMYWEAPON_H_
#include "weapon.h"

namespace sprite {

class MummyWeapon : public Weapon {
	public:
	MummyWeapon(Creature* owner) : Weapon(owner) {}

	virtual float range(){ return 1.0f; }
	virtual void Attack();

};

}//namespace

#endif /* MUMMYWEAPON_H_ */
