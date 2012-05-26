#ifndef HORUSEYE_COMPONENT_FOLLOWER_H_
#define HORUSEYE_COMPONENT_FOLLOWER_H_

#include "game/components/logic.h"
#include "game/sprites.h"

namespace component {

class Follower : public Logic {
  public:
    Follower(sprite::WorldObject* owner, sprite::WorldObject* target)
        : owner_(owner), target_(target) {}
    ~Follower() {}

    void Update(double dt);

  protected:
    sprite::WorldObject* owner_;
    sprite::WorldObject* target_;

};  // class Follower

}  // namespace component

#endif  // HORUSEYE_COMPONENT_FOLLOWER_H_
