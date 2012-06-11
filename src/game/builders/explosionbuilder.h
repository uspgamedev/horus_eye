#ifndef HORUSEYE_GAME_BUILDERS_EXPLOSIONBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_EXPLOSIONBUILDER_H_

#include "game/sprites.h"

namespace builder {

class ExplosionBuilder {
  public:
    ExplosionBuilder() {}
    ~ExplosionBuilder() {}

    sprite::WorldObject* FireballExplosion();
    sprite::WorldObject* EarthquakeExplosion();
    sprite::WorldObject* MeteorExplosion();
	sprite::WorldObject* MeleeExplosion(double damage, double range);
};

}

#endif /* HORUSEYE_GAME_BUILDERS_EXPLOSIONBUILDER_H_ */
