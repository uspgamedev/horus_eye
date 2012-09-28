#ifndef HORUSEYE_COMPONENT_CASTER_H_
#define HORUSEYE_COMPONENT_CASTER_H_

#include <map>
#include <vector>
#include <cstdlib>

#include <ugdk/util/idgenerator.h>

#include "game/sprites.h"

#include "game/components/controller.h"
#include "game/resources/resource.h"
#include "game/resources/energy.h"
#include "game/resources/capacityblocks.h"
#include "game/skills/usearguments.h"
#include "game/skills.h"

namespace component {

class Caster {
  public:
    Caster(sprite::WorldObject* owner, const resource::Energy& mana, int block_count,
        const skills::usearguments::Aim& aim);

    /// Easy constructor. Default block_count and aim.
    /**
     * Default block_count: 1
     * Default aim: from owner position, to owner's controller aim_destination
     */
    Caster(sprite::WorldObject* owner, const resource::Energy& mana);

    ~Caster();

    // METHODS

    void Update(double dt);

    bool CastSkill(Controller::SkillSlot slot);

    skills::Skill* SkillAt(Controller::SkillSlot slot);

    int LearnSkill(skills::Skill* skill);

    void UnlearnSkill(int id);

    /// Invalid id means unequip given slot.
    void EquipSkill(int id, Controller::SkillSlot);

    int LearnAndEquipSkill(skills::Skill* skill, Controller::SkillSlot slot) {
        int id = LearnSkill(skill);
        EquipSkill(id, slot);
        return id;
    }

    /// Returns your maximum mana when with all mana blocks.
    double FullMana() { return mana_blocks_.TotalCapcity(); }

    // GETTERS
    sprite::WorldObject* owner() const { return owner_; }

          resource::Energy& mana()       { return mana_; }
    const resource::Energy& mana() const { return mana_; }

          resource::CapacityBlocks& mana_blocks()       { return mana_blocks_; }
    const resource::CapacityBlocks& mana_blocks() const { return mana_blocks_; }
    
    const skills::usearguments::Aim& aim() const { return aim_; }

    size_t num_skills() const { return skills_.size(); }

          resource::Resource<int>& power()       { return power_; }
    const resource::Resource<int>& power() const { return power_; }

    /// Returns the current maximum mana.
    double max_mana() const { return mana_.max_value(); }

    // SETTERS
    void set_mana(resource::Energy &mana) { mana_ = mana; }
    void set_mana(double mana) { mana_.Set(mana); }

  protected:
    void unequipSkill(skills::Skill* skill);

  private:
    /// The owner.
    sprite::WorldObject* owner_;

    /// The known skills.
    std::vector<skills::Skill*> skills_;

    /// The active skills this caster has.
    std::map<Controller::SkillSlot, skills::Skill*> active_skills_;

    /// The mana of this caster. An energy manages reneration.
    resource::Energy mana_;

    /// Mana is separated in different blocks.
    resource::CapacityBlocks mana_blocks_;

    /// Where this caster is aiming.
    skills::usearguments::Aim aim_;

    /// TODO
    ugdk::util::IDGenerator skill_id_generator_;

    ///
    resource::Resource<int> power_;

};  // class Caster

}  // namespace component

#endif  // HORUSEYE_COMPONENT_CASTER_H_
