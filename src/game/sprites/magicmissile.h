#ifndef HORUSEYE_GAME_SPRITES_MAGICMISSILE_H_
#define HORUSEYE_GAME_SPRITES_MAGICMISSILE_H_

#include "projectile.h"

namespace sprite {

class MagicMissile : public Projectile {
  DEFINE_COLLIDABLE
  public:
    MagicMissile(ugdk::Vector2D &);
	~MagicMissile() {}
};

}

#endif /* HORUSEYE_GAME_SPRITES_MAGICMISSILE_H_ */
