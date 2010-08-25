/*
 * potionbuilder.h
 *
 *  Created on: 21/08/2010
 *      Author: Henrique
 */

#ifndef POTIONBUILDER_H_
#define POTIONBUILDER_H_

#include "item.h"

namespace sprite {

class ItemBuilder {
  public:
    ItemBuilder() {}
    ~ItemBuilder() {}

    Item* life_potion();
    Item* mana_potion();

  protected:

    class RecoverLifeEvent : public ItemEvent {

      public:

        RecoverLifeEvent (int recover) : recover_(recover) {}
        bool Use (Hero *);

      private:

        int recover_;

    };
    class RecoverManaEvent : public ItemEvent {

      public:

        RecoverManaEvent (int recover) : recover_(recover) {}
        bool Use (Hero *);

      private:

        int recover_;

    };

};

}

#endif /* POTIONBUILDER_H_ */
