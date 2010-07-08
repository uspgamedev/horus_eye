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
class Hero : public Creature, public Observer {

  public:
    Hero(Image* img = NULL);
    ~Hero() {} 

    virtual void CollidesWith(Mummy *);
    virtual void HandleCollision(WorldObject *);

  private:
    Vector2D screen_center_;
    bool pressed_key_[4];

    virtual void Update(float delta_t);
    double GetAttackingAngle(Vector2D mousePosition);
    int GetAttackingAnimationIndex(double angle);
    void StartAttack();
    void GetMouseState();
    void GetKeys();
    void Tick();
};

}
#endif // HERO_H_
