#ifndef HORUSEYE_GAME_BUILDERS_ENTITYBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_ENTITYBUILDER_H_

#include "game/sprites/worldobject.h"

namespace builder {

class EntityBuilder {
  public:
    EntityBuilder() {}
    ~EntityBuilder() {}

    sprite::WorldObject* BlueShieldEntity(sprite::WorldObject *target);
    sprite::WorldObject* FireballExplosion();
    sprite::WorldObject* EarthquakeExplosion();
    sprite::WorldObject* MeteorExplosion();
};

}

#endif /* HORUSEYE_GAME_SPRITES_CONDITIONBUILDER_H_ */
