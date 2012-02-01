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

    sprite::Mummy *StandingMummy(ugdk::FlexibleSpritesheet* image);
    sprite::Mummy *WalkingMummy(ugdk::FlexibleSpritesheet* image);
    sprite::Mummy *RangedMummy(ugdk::FlexibleSpritesheet* image);
    sprite::Mummy *StandingRangedMummy(ugdk::FlexibleSpritesheet* image);
    sprite::Mummy *BigMummy(ugdk::FlexibleSpritesheet* image);
    sprite::Mummy *StandingBigMummy(ugdk::FlexibleSpritesheet* image);
	sprite::Pharaoh *WalkingPharaoh(ugdk::FlexibleSpritesheet* image);
	sprite::Pharaoh *StandingPharaoh(ugdk::FlexibleSpritesheet* image);

};

} // namespace builder

#endif /* HORUS_EYE_GAME_BUILDERS_MUMMY_BUILDER_H_ */
