#ifndef HORUSEYE_COMPONENT_LOGIC_MUMMY_H_
#define HORUSEYE_COMPONENT_LOGIC_MUMMY_H_

#include "game/components/logic/creature.h"

namespace component {

class Mummy : public Creature {
  public:
    Mummy(sprite::WorldObject* owner);
    ~Mummy();
    
    void set_speed(double speed) { original_speed_ = speed_ = speed; }
    void OnWorldAdd(scene::World* world);

  protected:
    virtual void Update(double delta_t);

    void MummyAntiStack(sprite::WorldObject *);
    friend class MummyAntiStackCollision;
};

}
#endif // HORUSEYE_COMPONENT_LOGIC_MUMMY_H_
