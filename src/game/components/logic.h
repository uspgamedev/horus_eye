#ifndef HORUSEYE_COMPONENT_LOGIC_H_
#define HORUSEYE_COMPONENT_LOGIC_H_

#include "game/scenes.h"

namespace component {

class Logic {
  public:
    virtual ~Logic() {}

    virtual void Update(double dt) = 0;
    virtual void OnWorldAdd(scene::World* world) {}

  protected:
    Logic() {}
};

}
#endif // HORUSEYE_COMPONENT_LOGIC_H_
