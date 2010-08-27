//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/projectile.h
// Definicao da classe Explosion
//

#ifndef EXPLOSION_H_
#define EXPLOSION_H_

#include "../../framework/sprite.h"
#include "../../framework/engine.h"
#include "../../framework/timeaccumulator.h"
#include "../../framework/vector2D.h"

#include "worldobject.h"

namespace sprite {

using namespace framework;
class Mummy;
class Wall;
class Explosion : public WorldObject {

  public:
	Explosion(framework::Vector2D & pos, framework::Vector2D &);
	~Explosion();
	void Move(float delta_t);
	void RadiusUpdate(float delta_t);
	void Update(float delta_t);


	void CollidesWith(Wall *);
	void CollidesWith(Door * );
	void CollidesWith(Mummy *);
	virtual void HandleCollision(WorldObject *);
	int damage() { return damage_; }

  private:
	int damage_;
	float speed_;
    float radius_;
    bool exploding_;
    Vector2D direction_;
    TimeAccumulator *duration_;

    void Explode();
};

}

#endif /* EXPLOSION_H_ */
