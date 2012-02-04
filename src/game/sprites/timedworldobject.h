#ifndef HORUSEYE_GAME_SPRITE_TIMEDWORLDOBJECT_H_
#define HORUSEYE_GAME_SPRITE_TIMEDWORLDOBJECT_H_

#include <ugdk/time/timeaccumulator.h>
#include "game/sprites/worldobject.h"

namespace sprite {

class TimedWorldObject : public WorldObject {
  public:
  /** WorldObject that dies after a set duration.
    *
    * @param duration Life time of the WorldObject, in seconds.
    */
    TimedWorldObject(float duration);
    virtual ~TimedWorldObject();

    // Inherited Virtuals
    virtual void Update(float dt);
    virtual void Die() { super::Die(); };

  protected:
	ugdk::time::TimeAccumulator *timed_life_;

  private:
    typedef WorldObject super;

};  // class WorldObject

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_TIMEDWORLDOBJECT_H_
