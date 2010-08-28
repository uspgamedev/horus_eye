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
#include "../../framework/image.h"
#include "../../framework/vector2D.h"

namespace sprite {

class Mummy;
class Item;
class MummyProjectile;
class Hero : public Creature {

  public:
    Hero(framework::Image* img = NULL);
    ~Hero() {} 
    void TakeDamage(int life_points = 1);
    virtual void CollidesWith(Mummy *);
    virtual void CollidesWith(Item *);
    virtual void CollidesWith(MummyProjectile *);
    virtual void HandleCollision(WorldObject *);

  private:
    framework::Vector2D screen_center_;
    bool pressed_key_[4];
    double blink_time_;
    bool blink_;
    float time_to_recover_speed_;

    virtual void Update(float delta_t);
    virtual void Render(framework::Image *back_buffer, framework::Vector2D &offset);
    void StartAttack();
    void StartExplosion();
    int GetMouseState();
    void GetKeys();
};

}
#endif // HORUSEYE_GAME_SPRITE_HERO_H_
