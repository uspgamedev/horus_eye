#ifndef HORUSEYE_GAME_SPRITES_MUMMY_PROJECTILE_H_
#define HORUSEYE_GAME_SPRITES_MUMMY_PROJECTILE_H_

#include "projectile.h"

namespace ugdk {
class TimeAccumulator;
}

namespace sprite {

class Mummy;
class Wall;
class Door;
class MummyProjectile : public Projectile {

  public:
	MummyProjectile(ugdk::Vector2D &, int damage = 1);
	~MummyProjectile();

	virtual void CollidesWith(Hero * obj);
	
};

}

#endif /*MUMMY_PROJECTILE_H_ */
