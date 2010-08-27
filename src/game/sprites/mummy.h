//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/worldobject.h
// Definicao da classe Mummy.
//

#ifndef HORUSEYE_GAME_SPRITE_MUMMY_H_
#define HORUSEYE_GAME_SPRITE_MUMMY_H_


#include "creature.h"
#include "../../framework/animation.h"
#include "../../framework/vector2D.h"
#include "../../framework/timeaccumulator.h"
#include "../utils/circleobject.h"
#include <queue>

#define pbb pair<bool, bool>
#define TIME_TO_THINK 0.1

using namespace std;
using namespace framework;

namespace sprite {

class Mummy : public Creature {

  public:
    Mummy(Image* img = NULL);
    ~Mummy();
    virtual void HandleCollision(WorldObject *);
    virtual void CollidesWith(Projectile *);
    virtual void CollidesWith(Hero *);
    virtual void CollidesWith(Mummy *);
    
    void set_speed(float speed) { original_speed_ = speed_ = speed; }
    void set_life(int life) { life_ = life; }
    void set_bound(float radius) {
		if(bound_ != NULL)
			static_cast<CircleObject*>(bound_)->set_radius(radius);
		bound_ = new CircleObject(radius);

	}

    void StartAttack(Creature* obj);
  private:

    TimeAccumulator *interval_;
    float time_to_think_;
    bool standing_;
    Vector2D last_direction_;
    queue<Vector2D> path_;

    virtual void Update(float delta_t);
    void Think(float dt);
    void UpdateDirection(Vector2D destiny);
    void RandomMovement();
    void PlayHitSound() const;
};

}
#endif // MUMMY_H_
