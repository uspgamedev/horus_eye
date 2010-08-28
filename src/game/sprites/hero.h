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
#include <map>

namespace sprite {

class Mummy;
class MummyProjectile;
class Hero : public Creature {

  public:
    Hero(framework::Image* img = NULL);
    ~Hero() {} 
    void TakeDamage(int life_points = 1);
    virtual void CollidesWith(Mummy *);
    virtual void CollidesWith(MummyProjectile *);
    virtual void HandleCollision(WorldObject *);
    void AddWeapon(int slot, Weapon* weapon);
    void StartAttack();

  private:
    framework::Vector2D screen_center_;
    bool pressed_key_[4];
    double blink_time_;
    bool blink_;
    float time_to_recover_speed_;
    std::map<int, Weapon*> weapons_;
    int slot_selected_;

    virtual void Update(float delta_t);
    virtual void Render(framework::Image *back_buffer, framework::Vector2D &offset);
    void StartExplosion();
    int GetMouseState();
    void GetKeys();
    void ChangePrimaryWeapon(int slot);
};

}
#endif // HORUSEYE_GAME_SPRITE_HERO_H_
