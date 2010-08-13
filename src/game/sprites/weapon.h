#ifndef HORUSEYE_GAME_SPRITES_WEAPON_H_
#define HORUSEYE_GAME_SPRITES_WEAPON_H_

namespace sprite {
class Creature;

class Weapon {
	public:
	Weapon(Creature* owner){
		owner_ = owner;
	}
	virtual float range() = 0;
	virtual void Attack() = 0;

	protected:
	Creature* owner_;
};

}//namespace

#endif /* WEAPON_H_ */
