#ifndef MUMMY_BUILDER_H_
#define MUMMY_BUILDER_H_

#include "mummy.h"
#include "pharaoh.h"
#include "../utils/constants.h"

namespace framework{
	class Image;
}

namespace sprite {

class MummyBuilder {

  public:
    MummyBuilder() {}

    Mummy *StandingMummy(framework::Image* image);
    Mummy *WalkingMummy(framework::Image* image);
    Mummy *RangedMummy(framework::Image* image);
    Mummy *StandingRangedMummy(framework::Image* image);
    Mummy *BigMummy(framework::Image* image);
    Mummy *StandingBigMummy(framework::Image* image);
	Pharaoh *WalkingPharaoh(framework::Image* image);
	Pharaoh *StandingPharaoh(framework::Image* image);

};

}

#endif /* MUMMY_BUILDER_H_ */
