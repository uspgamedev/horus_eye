//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/projectile.h
// Definicao da classe Projectile
//

#ifndef HORUSEYE_GAME_SPRITES_PROJECTILE_H_
#define HORUSEYE_GAME_SPRITES_PROJECTILE_H_

#include "worldobject.h"
#include "../../framework/vector2D.h"

namespace framework {
class TimeAccumulator;
}

namespace sprite {

class Hero;
class Mummy;
class Wall;
class Door;
class Creature;

class Projectile : public WorldObject {
  public:
    Projectile(int damage, float speed, int duration, framework::Vector2D &);
    virtual ~Projectile();
    void Move(float delta_t);
    void Update(float delta_t);

    virtual void CollidesWith(Hero * obj) {}
    virtual void CollidesWith(Mummy * obj) {}
    virtual void CollidesWith(Wall * obj) { Explode(); }
    virtual void CollidesWith(Door * obj) { Explode(); }
    virtual void HandleCollision(WorldObject *);
    int damage() { return damage_; }

  protected:
    int damage_;
    float speed_;
    framework::Vector2D direction_;
    framework::TimeAccumulator *duration_;
    bool exploding_;

    void Explode();
};

}

#endif /* HORUSEYE_GAME_SPRITES_PROJECTILE_H_ */
