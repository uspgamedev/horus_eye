#ifndef HORUSEYE_GAME_SPRITES_ITEMBUILDER_H_
#define HORUSEYE_GAME_SPRITES_ITEMBUILDER_H_

#include "item.h"
#include "condition.h"
#include "conditionbuilder.h"
#include "../../framework/image.h"

namespace sprite {

class ItemBuilder {
  public:
    ItemBuilder() {}
    ~ItemBuilder() {}

    Item* LifePotion(framework::Image* image);
    Item* ManaPotion(framework::Image* image);
    Item* SightPotion(framework::Image* image);

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
    class IncreaseSightEvent : public ItemEvent {
      public:
        IncreaseSightEvent (float additional_sight) : additional_sight_(additional_sight) {}
        bool Use (Hero *);

      private:
        float additional_sight_;
        ConditionBuilder condition_builder_;
    };
};

}

#endif /* HORUSEYE_GAME_SPRITES_ITEMBUILDER_H_ */
