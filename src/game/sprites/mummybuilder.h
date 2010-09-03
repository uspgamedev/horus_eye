#ifndef MUMMY_BUILDER_H
#define MUMMY_BUILDER_H_

#include "mummy.h"
#include "pharaoh.h"
#include "../utils/constants.h"

namespace sprite {

class MummyBuilder {

  public:
    MummyBuilder() {}

    Mummy *standard_mummy();
    Mummy *ranged_mummy();
    Mummy *big_mummy();
	Pharaoh *pharaoh();

};

}

#endif /* MUMMY_BUILDER_H_ */
