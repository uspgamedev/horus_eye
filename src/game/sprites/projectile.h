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

namespace sprite {

class Mummy;
class Wall;
class Projectile : public WorldObject {

  public:
	Projectile(framework::Vector2D & pos, framework::Vector2D &);
	void Move(float delta_t);
	void Update(float delta_t);

	void CollidesWith(Mummy * obj); // TODO: implementar quando existir Mummy
	void CollidesWith(Wall * obj);
	int dano() { return dano_; }

  private:
	int dano_;
    float speed_;
    framework::Vector2D direction_;
    framework::TimeAccumulator duration_;
};

}

#endif /* PROJECTILE_H_ */
