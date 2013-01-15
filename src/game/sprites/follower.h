#ifndef HORUSEYE_GAME_SPRITE_FOLLOWER_H_
#define HORUSEYE_GAME_SPRITE_FOLLOWER_H_

#include <ugdk/action.h>
#include "game/sprites/worldobject.h"

namespace sprite {

class Follower : public WorldObject {
  typedef WorldObject super;
  public:
    Follower(WorldObject* target, double duration = -1.0);
    virtual ~Follower() {}

    virtual void Update(double dt);

    void set_collision_object(pyramidworks::collision::CollisionObject* col) { collision_object_ = col; }

  protected:
    WorldObject* target_;

};  // class WorldObject

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
