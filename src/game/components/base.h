#ifndef HORUSEYE_COMPONENT_BASE_H_
#define HORUSEYE_COMPONENT_BASE_H_

#include <game/sprites.h>

namespace component {

class Base {
  public:
    virtual ~Base() {}

    virtual void Update(double dt) = 0;

    /// Called when the component is added to an object.
    virtual void OnAdd(sprite::WorldObject*) {}

  protected:
    Base() {}
};

}
#endif // HORUSEYE_COMPONENT_BASE_H_
