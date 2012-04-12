#ifndef HORUSEYE_COMPONENT_BASECOMPONENT_H_
#define HORUSEYE_COMPONENT_BASECOMPONENT_H_

#include "game/sprites.h"

namespace component {

class BaseComponent { 
  public:
    virtual ~BaseComponent() {}

          sprite::WorldObject* owner()       { return owner_; }
    const sprite::WorldObject* owner() const { return owner_; }

  protected:
    BaseComponent(sprite::WorldObject* owner) : owner_(owner) {}

    /// The owner.
    sprite::WorldObject* owner_;

};  // class BaseComponent

}  // namespace component

#endif  // HORUSEYE_COMPONENT_BASECOMPONENT_H_
