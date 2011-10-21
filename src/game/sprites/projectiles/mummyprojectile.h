#ifndef HORUSEYE_GAME_SPRITES_MUMMY_PROJECTILE_H_
#define HORUSEYE_GAME_SPRITES_MUMMY_PROJECTILE_H_

#include "game/sprites/projectiles/projectile.h"

namespace sprite {

class MummyProjectile : public Projectile {
  public:
	MummyProjectile(ugdk::Vector2D &, int damage = 1);
    ~MummyProjectile() {}
};

}

#endif /* HORUSEYE_GAME_SPRITES_MUMMY_PROJECTILE_H_ */
