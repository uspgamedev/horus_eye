//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/projectile.h
// Definicao da classe Explosion
//

#ifndef HORUSEYE_GAME_SPRITES_EXPLOSION_H_
#define HORUSEYE_GAME_SPRITES_EXPLOSION_H_

#include "worldobject.h"
#include "../../framework/vector2D.h"
#include <set>

namespace framework {
class TimeAccumulator;
}

namespace sprite {

class Wall;
class Door;
class Mummy;
class Explosion : public WorldObject {

  public:
	Explosion(framework::Vector2D &);
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
    framework::Vector2D direction_;
    framework::TimeAccumulator *duration_;
    std::set<Mummy*> already_hit_;

    void Explode();
};

}

#endif /* HORUSEYE_GAME_SPRITES_EXPLOSION_H_ */
