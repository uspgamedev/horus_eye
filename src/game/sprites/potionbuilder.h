/*
 * potionbuilder.h
 *
 *  Created on: 21/08/2010
 *      Author: Henrique
 */

#ifndef POTIONBUILDER_H_
#define POTIONBUILDER_H_

#include "potion.h"

namespace sprite {

class PotionBuilder {
  public:
    PotionBuilder() {}
    ~PotionBuilder() {}

    Potion* life_potion();
    Potion* mana_potion();
};

}

#endif /* POTIONBUILDER_H_ */
