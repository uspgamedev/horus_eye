#ifndef HORUSEYE_COMPONENT_LOGIC_MUMMY_H_
#define HORUSEYE_COMPONENT_LOGIC_MUMMY_H_

#include "game/components/logic/creature.h"

namespace component {

class Mummy : public Creature {
  public:
    Mummy(sprite::WorldObject* owner, double speed);
    ~Mummy();
    void OnWorldAdd(scene::World* world);
};

}
#endif // HORUSEYE_COMPONENT_LOGIC_MUMMY_H_
