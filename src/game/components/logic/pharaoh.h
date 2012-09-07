#ifndef HORUSEYE_COMPONENT_LOGIC_PHARAOH_H_
#define HORUSEYE_COMPONENT_LOGIC_PHARAOH_H_

#include "game/components/logic/mummy.h"
#include "game/skills/combatart.h"
#include "game/skills.h"

namespace component {

class Pharaoh : public Mummy {

  public:
    Pharaoh(sprite::WorldObject* owner);
    ~Pharaoh();

  protected:
    void Think(double dt);

};

}
#endif // HORUSEYE_COMPONENT_LOGIC_PHARAOH_H_