#ifndef HORUSEYE_COMPONENT_LOGIC_HERO_H_
#define HORUSEYE_COMPONENT_LOGIC_HERO_H_

#include <map>
#include <ugdk/math/vector2D.h>

#include "game/components/logic/creature.h"
#include "game/resources/resource.h"
#include "game/resources/capacityblocks.h"

namespace skills {
class Skill;
} // skills

namespace component {

class Hero : public Creature {
  public:
    Hero(sprite::WorldObject* owner, double speed);
    ~Hero();

    void SetupCollision();
    
  private:
    void CollisionSlow();
    void AddKnownCollisions();

    friend class MummySlowCollision;
};

}
#endif // HORUSEYE_COMPONENT_LOGIC_HERO_H_
