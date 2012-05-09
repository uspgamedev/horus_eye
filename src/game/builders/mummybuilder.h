#ifndef HORUS_EYE_GAME_BUILDERS_MUMMY_BUILDER_H_
#define HORUS_EYE_GAME_BUILDERS_MUMMY_BUILDER_H_

#include <ugdk/graphic.h>
#include "game/sprites.h"

namespace builder {
class MummyBuilder {
  public:
    MummyBuilder();

    sprite::WorldObject *StandingMummy(ugdk::graphic::FlexibleSpritesheet* image);
    sprite::WorldObject *WalkingMummy(ugdk::graphic::FlexibleSpritesheet* image);
    sprite::WorldObject *RangedMummy(ugdk::graphic::FlexibleSpritesheet* image);
    sprite::WorldObject *StandingRangedMummy(ugdk::graphic::FlexibleSpritesheet* image);
    sprite::WorldObject *BigMummy(ugdk::graphic::FlexibleSpritesheet* image);
    sprite::WorldObject *StandingBigMummy(ugdk::graphic::FlexibleSpritesheet* image);
	sprite::WorldObject *WalkingPharaoh(ugdk::graphic::FlexibleSpritesheet* image);
	sprite::WorldObject *StandingPharaoh(ugdk::graphic::FlexibleSpritesheet* image);

};

} // namespace builder

#endif /* HORUS_EYE_GAME_BUILDERS_MUMMY_BUILDER_H_ */
