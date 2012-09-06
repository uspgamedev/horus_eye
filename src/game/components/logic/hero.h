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
    Hero(sprite::WorldObject* owner);
    ~Hero();

    void AddWeapon(int slot, skills::Skill* combat_art);
    void StartAttackAnimation();

    size_t num_skills() const { return skills_.size(); }
    bool ChangeSecondaryWeapon(int slot);
    skills::Skill* secondary_combat_art();

    void SetupCollision();
    
  private:
    std::map<int, skills::Skill*> skills_;
    int slot_selected_;

    virtual void Update(double delta_t);
   
    void CollisionSlow();
    void AddKnownCollisions();

    friend class MummySlowCollision;
};

}
#endif // HORUSEYE_COMPONENT_LOGIC_HERO_H_
