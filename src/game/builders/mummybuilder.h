#ifndef HORUS_EYE_GAME_BUILDERS_MUMMY_BUILDER_H_
#define HORUS_EYE_GAME_BUILDERS_MUMMY_BUILDER_H_

namespace ugdk {
class FlexibleSpritesheet;
}
namespace sprite {
class Mummy;
class Pharaoh;
}

namespace builder {

class MummyBuilder {
  public:
    MummyBuilder() {}

    sprite::Mummy *StandingMummy(ugdk::graphic::FlexibleSpritesheet* image);
    sprite::Mummy *WalkingMummy(ugdk::graphic::FlexibleSpritesheet* image);
    sprite::Mummy *RangedMummy(ugdk::graphic::FlexibleSpritesheet* image);
    sprite::Mummy *StandingRangedMummy(ugdk::graphic::FlexibleSpritesheet* image);
    sprite::Mummy *BigMummy(ugdk::graphic::FlexibleSpritesheet* image);
    sprite::Mummy *StandingBigMummy(ugdk::graphic::FlexibleSpritesheet* image);
	sprite::Pharaoh *WalkingPharaoh(ugdk::graphic::FlexibleSpritesheet* image);
	sprite::Pharaoh *StandingPharaoh(ugdk::graphic::FlexibleSpritesheet* image);

};

} // namespace builder

#endif /* HORUS_EYE_GAME_BUILDERS_MUMMY_BUILDER_H_ */
