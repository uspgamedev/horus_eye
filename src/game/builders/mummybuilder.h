#ifndef HORUS_EYE_GAME_BUILDERS_MUMMY_BUILDER_H_
#define HORUS_EYE_GAME_BUILDERS_MUMMY_BUILDER_H_

namespace ugdk {
class Image;
}
namespace sprite {
class Mummy;
class Pharaoh;
}

namespace builder {

class MummyBuilder {
  public:
    MummyBuilder() {}

    sprite::Mummy *StandingMummy(ugdk::Image* image);
    sprite::Mummy *WalkingMummy(ugdk::Image* image);
    sprite::Mummy *RangedMummy(ugdk::Image* image);
    sprite::Mummy *StandingRangedMummy(ugdk::Image* image);
    sprite::Mummy *BigMummy(ugdk::Image* image);
    sprite::Mummy *StandingBigMummy(ugdk::Image* image);
	sprite::Pharaoh *WalkingPharaoh(ugdk::Image* image);
	sprite::Pharaoh *StandingPharaoh(ugdk::Image* image);

};

} // namespace builder

#endif /* HORUS_EYE_GAME_BUILDERS_MUMMY_BUILDER_H_ */
