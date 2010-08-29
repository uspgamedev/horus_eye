//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/pharaoh.h
// Definicao da classe Pharaoh.
//

#ifndef HORUSEYE_GAME_SPRITE_PHARAOH_H_
#define HORUSEYE_GAME_SPRITE_PHARAOH_H_

#include <queue>
#include "creature.h"
#include "../../framework/vector2D.h"
#include "../utils/circleobject.h"

#define pbb pair<bool, bool>
#define PHARAOH_TIME_TO_THINK 0.05f

namespace framework {
class TimeAccumulator;
}

namespace sprite {

class Weapon;
class Explosion;

class Pharaoh : public Mummy {

  public:
    Pharaoh(int life);
    ~Pharaoh();

	void set_ranged_weapon(Weapon *weapon) { ranged_weapon_ = weapon; }

    virtual void HandleCollision(WorldObject *);
    virtual void CollidesWith(Projectile *);
    virtual void CollidesWith(Explosion *);
    virtual void CollidesWith(Hero *);
    virtual void CollidesWith(Mummy *);
    
    void StartAttack(Creature* obj);

  protected:
	Weapon *ranged_weapon_;

    void Think(float dt);

};

}
#endif // PHARAOH_H_
