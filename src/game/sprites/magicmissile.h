//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/projectile.h
// Definicao da classe Projectile
//

#ifndef HORUSEYE_GAME_SPRITES_MAGICMISSILE_H_
#define HORUSEYE_GAME_SPRITES_MAGICMISSILE_H_
#include <set>

#include "projectile.h"
#include "../../framework/vector2D.h"

namespace framework {
class TimeAccumulator;
}

namespace sprite {

class Mummy;
class Wall;
class Door;
class Creature;

class MagicMissile : public Projectile {
  public:
    MagicMissile(framework::Vector2D &);
    ~MagicMissile();

    virtual void CollidesWith(Mummy * obj);
    virtual void HandleCollision(WorldObject *);
};

}

#endif /* HORUSEYE_GAME_SPRITES_MAGICMISSILE_H_ */
