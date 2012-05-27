#ifndef HORUSEYE_COMPONENT_LOGIC_PHARAOH_H_
#define HORUSEYE_COMPONENT_LOGIC_PHARAOH_H_

#include "game/components/logic/mummy.h"
#include "game/skills/combatart.h"
#include "game/skills/usearguments.h"

namespace ugdk {
class Image;
class Vector2D;
}

namespace skills {
class Skill;
}

namespace component {

class Pharaoh : public Mummy {

  public:
    Pharaoh(sprite::WorldObject* owner, int mana);
    ~Pharaoh();

    void set_ranged_weapon(skills::Skill *weapon) { ranged_weapon_ = weapon; }
    void set_summon_weapon(skills::Skill *weapon) { summon_weapon_ = weapon; }

    void StartSummonMummy(component::Creature* target);

  protected:
    skills::Skill *ranged_weapon_, *summon_weapon_;
    void Think(double dt);

};

}
#endif // HORUSEYE_COMPONENT_LOGIC_PHARAOH_H_