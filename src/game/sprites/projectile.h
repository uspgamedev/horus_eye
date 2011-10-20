#ifndef HORUSEYE_GAME_SPRITES_PROJECTILE_H_
#define HORUSEYE_GAME_SPRITES_PROJECTILE_H_

#include "worldobject.h"
#include <ugdk/math/vector2D.h>

namespace ugdk {
class TimeAccumulator;
}

namespace sprite {

class Hero;
class Mummy;
class Wall;
class Door;
class Block;
class Creature;

class Projectile : public WorldObject {
  public:
    Projectile(int damage, float speed, int duration, ugdk::Vector2D &);
    virtual ~Projectile();
    void Move(float delta_t);
    void Update(float delta_t);

    virtual void CollidesWith(Hero * obj) {}
    virtual void CollidesWith(Mummy * obj) {}
    virtual void CollidesWith(Wall * obj) { Explode(); }
    virtual void CollidesWith(Door * obj) { Explode(); }
    virtual void CollidesWith(Block * obj) { Explode(); }
    virtual void HandleCollision(WorldObject *);
    int damage() { return damage_; }

  protected:
    int damage_;
    float speed_;
    ugdk::Vector2D direction_;
    ugdk::TimeAccumulator *duration_;
    bool exploding_;

    virtual void Explode();
};

}

#endif /* HORUSEYE_GAME_SPRITES_PROJECTILE_H_ */
