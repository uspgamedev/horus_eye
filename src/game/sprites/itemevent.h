#ifndef HORUSEYE_GAME_SPRITES_ITEMEVENT_H_
#define HORUSEYE_GAME_SPRITES_ITEMEVENT_H_

#include "game/sprites.h"

namespace sprite {

class ItemEvent {
  public:
    virtual ~ItemEvent () {}
    virtual bool Use (WorldObject *) = 0;
  protected:
    ItemEvent () {}
};

}

#endif /* HORUSEYE_GAME_SPRITES_ITEMEVENT_H_ */
