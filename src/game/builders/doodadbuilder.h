#ifndef HORUSEYE_GAME_BUILDERS_DOODADBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_DOODADBUILDER_H_

#include "game/scenes.h"
#include "game/sprites.h"

namespace builder {

class DoodadBuilder {
  public:
    DoodadBuilder() {}
    ~DoodadBuilder() {}

    sprite::WorldObject* Door(scene::World* world);
    sprite::WorldObject* Wall();
    sprite::WorldObject* Entry();
    sprite::WorldObject* Button();
	sprite::WorldObject* Block();
};

}

#endif /* HORUSEYE_GAME_BUILDERS_DOODADBUILDER_H_ */
