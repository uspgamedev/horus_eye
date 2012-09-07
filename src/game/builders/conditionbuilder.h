#ifndef HORUSEYE_GAME_SPRITES_CONDITIONBUILDER_H_
#define HORUSEYE_GAME_SPRITES_CONDITIONBUILDER_H_

#include "game/sprites/condition.h"

namespace builder {

class ConditionBuilder {
  public:
    ConditionBuilder() {}
    ~ConditionBuilder() {}

    sprite::Condition* increase_sight_condition(sprite::WorldObject *owner);
};

}

#endif /* HORUSEYE_GAME_SPRITES_CONDITIONBUILDER_H_ */
