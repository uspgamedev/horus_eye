#ifndef HORUSEYE_COMPONENT_LOGIC_H_
#define HORUSEYE_COMPONENT_LOGIC_H_

#include "game/components/base.h"

#include "game/map.h"

namespace component {

class Logic : public Base {
  public:
    virtual ~Logic() {}

    virtual void Update(double dt) = 0;
    virtual void OnRoomAdd(map::Room* world) {}

  protected:
    Logic() : Base("logic") {}
};

}
#endif // HORUSEYE_COMPONENT_LOGIC_H_
