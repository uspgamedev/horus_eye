//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/projectile.h
// Definicao da classe Projectile
//

#ifndef HORUSEYE_GAME_SPRITES_MUMMY_PROJECTILE_H_
#define HORUSEYE_GAME_SPRITES_MUMMY_PROJECTILE_H_

#include "projectile.h"

namespace framework {
class TimeAccumulator;
}

namespace sprite {

class Mummy;
class Wall;
class Door;
class MummyProjectile : public Projectile {

  public:
	MummyProjectile(framework::Vector2D &, int damage = 1);
	~MummyProjectile();

	virtual void CollidesWith(Hero * obj);
	virtual void HandleCollision(WorldObject *);
};

}

#endif /*MUMMY_PROJECTILE_H_ */
