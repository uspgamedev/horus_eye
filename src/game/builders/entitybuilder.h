#ifndef HORUSEYE_GAME_BUILDERS_ENTITYBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_ENTITYBUILDER_H_

#include "game/scenes.h"
#include "game/sprites.h"

namespace builder {

class EntityBuilder {
  public:
    EntityBuilder() {}
    ~EntityBuilder() {}

    sprite::WorldObject* BlueShieldEntity(sprite::WorldObject *target);
    sprite::WorldObject* Door(scene::World* world);
    sprite::WorldObject* Wall();
    sprite::WorldObject* Entry();
};

}

#endif /* HORUSEYE_GAME_SPRITES_CONDITIONBUILDER_H_ */
