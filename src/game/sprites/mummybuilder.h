#ifndef MUMMY_BUILDER_H
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

    Mummy *standard_mummy(framework::Image* image);
    Mummy *ranged_mummy(framework::Image* image);
    Mummy *big_mummy(framework::Image* image);
	Pharaoh *pharaoh(framework::Image* image);

};

}

#endif /* MUMMY_BUILDER_H_ */
