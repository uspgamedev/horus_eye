//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/projectile.h
// Definicao da classe Projectile
//

#ifndef HORUSEYE_GAME_SPRITES_MUMMY_PROJECTILE_H_
#define HORUSEYE_GAME_SPRITES_MUMMY_PROJECTILE_H_

#include "worldobject.h"

namespace framework {
class TimeAccumulator;
}

namespace sprite {

class Mummy;
class Wall;
class Door;
class MummyProjectile : public WorldObject {

  public:
	MummyProjectile(framework::Vector2D & pos, framework::Vector2D &, int damage = 1);
	~MummyProjectile();
	void Move(float delta_t);
	void Update(float delta_t);

	virtual void CollidesWith(Hero * obj);
	virtual void CollidesWith(Wall * obj);
    virtual void CollidesWith(Door * obj);
	virtual void HandleCollision(WorldObject *);
	int damage() { return damage_; }

  private:
	int damage_;
    float speed_;
    framework::Vector2D direction_;
    framework::TimeAccumulator *duration_;
};

}

#endif /*MUMMY_PROJECTILE_H_ */
