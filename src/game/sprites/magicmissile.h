#ifndef HORUSEYE_GAME_SPRITES_MAGICMISSILE_H_
#define HORUSEYE_GAME_SPRITES_MAGICMISSILE_H_
#include <set>

#include "projectile.h"
#include <ugdk/math/vector2D.h>

namespace ugdk {
class TimeAccumulator;
}

namespace sprite {

class Mummy;
class Wall;
class Door;
class Creature;

class MagicMissile : public Projectile {
  public:
    MagicMissile(ugdk::Vector2D &);
    ~MagicMissile();

    virtual void CollidesWith(Mummy * obj);
    virtual void HandleCollision(WorldObject *);
};

}

#endif /* HORUSEYE_GAME_SPRITES_MAGICMISSILE_H_ */
