//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/projectile.h
// Definicao da classe Projectile
//

#ifndef PROJECTILE_H_
#define PROJECTILE_H_

#include "../../framework/sprite.h"
#include "../../framework/engine.h"
#include "../../framework/timeaccumulator.h"

#include "worldobject.h"

using namespace framework;
namespace sprite {

class Mummy;
class Wall;
class Door;
class Projectile : public WorldObject {

  public:
	Projectile(framework::Vector2D & pos, framework::Vector2D &);
	~Projectile();
	void Move(float delta_t);
	void Update(float delta_t);

	virtual void CollidesWith(Mummy * obj);
	virtual void CollidesWith(Wall * obj);
    virtual void CollidesWith(Door * obj);
	virtual void HandleCollision(WorldObject *);
	int damage() { return damage_; }

  private:
	int damage_;
    float speed_;
    Vector2D direction_;
    TimeAccumulator *duration_;
};

}

#endif /* PROJECTILE_H_ */
