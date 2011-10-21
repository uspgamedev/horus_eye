#ifndef MUMMY_BUILDER_H_
#define MUMMY_BUILDER_H_

namespace ugdk{
class Image;
}

namespace sprite {
class Mummy;
class Pharaoh;
class MummyBuilder {
  public:
    MummyBuilder() {}

    Mummy *StandingMummy(ugdk::Image* image);
    Mummy *WalkingMummy(ugdk::Image* image);
    Mummy *RangedMummy(ugdk::Image* image);
    Mummy *StandingRangedMummy(ugdk::Image* image);
    Mummy *BigMummy(ugdk::Image* image);
    Mummy *StandingBigMummy(ugdk::Image* image);
	Pharaoh *WalkingPharaoh(ugdk::Image* image);
	Pharaoh *StandingPharaoh(ugdk::Image* image);

};

}

#endif /* MUMMY_BUILDER_H_ */
