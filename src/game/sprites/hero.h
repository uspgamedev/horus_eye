//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/worldobject.h
// Definicao da classe Hero.
//

#ifndef HORUSEYE_GAME_SPRITE_HERO_H_
#define HORUSEYE_GAME_SPRITE_HERO_H_


#include "creature.h"
#include "../../framework/animation.h"
#include "../../framework/vector2D.h"

using namespace std;
using namespace framework;

namespace sprite {

class Mummy;
class Hero : public Creature{

  public:
    Hero(Image* img = NULL);
    ~Hero() {} 
	void TakeDamage(int life_points = 1);
    virtual void CollidesWith(Mummy *);
    virtual void CollidesWith(LifePotion *);
    virtual void CollidesWith(MummyProjectile *);
    virtual void HandleCollision(WorldObject *);

  private:
    Vector2D screen_center_;
    bool pressed_key_[4];
    double blink_time_;
    bool blink_;
    float time_to_recover_speed_;

    virtual void Update(float delta_t);
    virtual void Render(Image *back_buffer, Vector2D &offset);
    void StartAttack();
    bool GetMouseState();
    void GetKeys();
};

}
#endif // HERO_H_
