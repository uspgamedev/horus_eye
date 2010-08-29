//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/worldobject.h
// Definicao da classe Mummy.
//

#ifndef HORUSEYE_GAME_SPRITE_MUMMY_H_
#define HORUSEYE_GAME_SPRITE_MUMMY_H_

#include <queue>
#include "creature.h"
#include "../../framework/vector2D.h"
#include "../utils/circleobject.h"

#define pbb pair<bool, bool>
#define TIME_TO_THINK 0.1f

namespace framework {
class TimeAccumulator;
}

namespace sprite {

class Weapon;

class Mummy : public Creature {

  public:
    Mummy(framework::Image* img = NULL);
    ~Mummy();

    virtual void HandleCollision(WorldObject *);
    virtual void CollidesWith(Mummy *);
    
    void set_speed(float speed) { original_speed_ = speed_ = speed; }
    void set_life(int life) { life_ = life; }
    void TakeDamage(int life_points = 1);
	void set_weapon(Weapon *weapon) { weapon_ = weapon; }
    void set_bound(float radius) {
		if(bound_ != NULL)
			delete bound_;
		bound_ = new utils::CircleObject(radius);
	}

    void StartAttack(Creature* obj);
  private:

    framework::TimeAccumulator *interval_;
    float time_to_think_;
    bool standing_;
    Vector2D last_direction_;
	Weapon *weapon_;
    queue<Vector2D> path_;

    virtual void Update(float delta_t);
    void Think(float dt);
    void UpdateDirection(Vector2D destiny);
    void RandomMovement();
    void PlayHitSound() const;
};

}
#endif // MUMMY_H_
