#ifndef HORUSEYE_GAME_SPRITE_TIMEDWORLDOBJECT_H_
#define HORUSEYE_GAME_SPRITE_TIMEDWORLDOBJECT_H_

#include <ugdk/time/timeaccumulator.h>
#include "game/sprites/worldobject.h"

namespace sprite {

class TimedWorldObject : public WorldObject {
  
  public:
    TimedWorldObject(float duration);
    virtual ~TimedWorldObject();

    // The BIG Awesome update method. TODO explain better
    virtual void Update(float dt);

  protected:
	ugdk::TimeAccumulator *timed_life_;

};  // class WorldObject

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_TIMEDWORLDOBJECT_H_
