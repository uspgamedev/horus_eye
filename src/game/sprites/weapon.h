#ifndef HORUSEYE_GAME_SPRITES_WEAPON_H_
#define HORUSEYE_GAME_SPRITES_WEAPON_H_
#include"../../framework/vector2D.h"
namespace sprite {
class Creature;
class Weapon {
	public:
	Weapon(Creature* owner){
		owner_ = owner;
	}
	virtual float range() = 0;
	virtual void Attack(framework::Vector2D target = framework::Vector2D(-1,-1)) = 0;

	protected:
	Creature* owner_;
};

}//namespace

#endif /* WEAPON_H_ */
