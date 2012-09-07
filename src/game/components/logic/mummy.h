#ifndef HORUSEYE_COMPONENT_LOGIC_MUMMY_H_
#define HORUSEYE_COMPONENT_LOGIC_MUMMY_H_

#include "game/components/logic/creature.h"

namespace component {

class Mummy : public Creature {
  public:
    Mummy(sprite::WorldObject* owner, double speed) : Creature(owner, speed) {}
    ~Mummy() {}
};

}
#endif // HORUSEYE_COMPONENT_LOGIC_MUMMY_H_
