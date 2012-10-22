#ifndef HORUSEYE_COMPONENT_LOGIC_FOLLOWER_H_
#define HORUSEYE_COMPONENT_LOGIC_FOLLOWER_H_

#include "game/components/base.h"

#include "game/sprites.h"

namespace component {

class Follower : public Base {
  public:
    static const char* DEFAULT_NAME() { return "follower"; }
    static int DEFAULT_ORDER() { return 0; }

    Follower(sprite::WorldObject* owner, sprite::WorldObject* target)
        : owner_(owner), target_(target) {}
    ~Follower() {}

    void Update(double dt);

  protected:
    sprite::WorldObject* owner_;
    sprite::WorldObject* target_;

};  // class Follower

}  // namespace component

#endif  // HORUSEYE_COMPONENT_LOGIC_FOLLOWER_H_
