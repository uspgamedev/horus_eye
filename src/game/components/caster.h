#ifndef HORUSEYE_COMPONENT_CASTER_H_
#define HORUSEYE_COMPONENT_CASTER_H_

#include <map>

#include "game/sprites.h"

#include "game/components/logic.h"
#include "game/components/controller.h"
#include "game/resources/energy.h"
#include "game/resources/capacityblocks.h"
#include "game/skills/usearguments.h"
#include "game/skills.h"

namespace component {

class Caster {
  public:
    Caster(sprite::WorldObject* owner, const resource::Energy& mana, int block_count, const skills::usearguments::Aim& aim);

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
    skills::Skill* SkillAt(Controller::SkillSlot slot) {
		return active_skills_[slot];
	}

    void AddSkill(int id, skills::Skill* skill);

    /// Invalid id means unequip given slot.
    void EquipSkill(int id, Controller::SkillSlot);

    /// Returns your maximum mana when with all mana blocks.
    double FullMana() { return mana_blocks_.TotalCapcity(); }

    // GETTERS

    sprite::WorldObject* owner() const { return owner_; }

    const skills::usearguments::Aim& aim() const { return aim_; }

          resource::Energy& mana()       { return mana_; }
    const resource::Energy& mana() const { return mana_; }

    resource::CapacityBlocks& mana_blocks() { return mana_blocks_; }

    size_t num_skills() const { return skills_.size(); }

    /// Returns the current maximum mana.
    double max_mana() const { return mana_.max_value(); }

    // SETTERS

    void set_mana(resource::Energy &mana) { mana_ = mana; }
    void set_mana(double mana) { mana_.Set(mana); }

  protected:
    /// The owner.
    sprite::WorldObject* owner_;

    std::map<int, skills::Skill*> skills_;

    /// The active weapons this creature has.
    std::map<Controller::SkillSlot, skills::Skill*> active_skills_;

    /// The mana of this creature. An energy manages reneration.
    resource::Energy mana_;

    /// TODO
    resource::CapacityBlocks mana_blocks_;

    /// An aim resource. It's origin points to the creature's position and the destination to the creature's aim.
    skills::usearguments::Aim aim_;

};  // class Caster

}  // namespace component

#endif  // HORUSEYE_COMPONENT_CASTER_H_
