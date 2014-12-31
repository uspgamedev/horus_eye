#ifndef HORUSEYE_COMPONENT_BASE_H_
#define HORUSEYE_COMPONENT_BASE_H_

#include <game/sprites.h>
#include <string>

namespace component {

class Base {
  public:
    virtual ~Base() {}

    virtual void Update(double dt) = 0;

    /// The name that the component is registered with on the object.
    virtual std::string component_name() const = 0;

    /// The order that the component is registered with on the object.
    virtual int order() const = 0;

    /// Called when the component is added to an object.
    virtual void OnAdd(sprite::WObjRawPtr) {}

    /// Called when the object is removed.
    virtual void OnObjectRemoved() {}

  protected:
    Base() {}
};

}
#endif // HORUSEYE_COMPONENT_BASE_H_
