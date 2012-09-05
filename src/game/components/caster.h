#ifndef HORUSEYE_COMPONENT_CASTER_H_
#define HORUSEYE_COMPONENT_CASTER_H_

#include <map>

#include "game/sprites.h"

#include "game/components/logic.h"
#include "game/components/controller.h"
#include "game/resources/energy.h"
#include "game/skills/usearguments.h"
#include "game/skills.h"

namespace component {

class Caster {
  public:
    Caster(sprite::WorldObject* owner, const skills::usearguments::Aim& aim);
    /// Default aim: from owner position, to owner's controller aim_destination
    Caster(sprite::WorldObject* owner);
    ~Caster();

    void Update(double dt);
    bool CastSkill(Controller::SkillSlot slot);
    skills::Skill* SkillAt(Controller::SkillSlot slot) {
		return active_skills_[slot];
	}

    sprite::WorldObject* owner() const { return owner_; }
    const skills::usearguments::Aim& aim() const { return aim_; }
    resource::Energy& mana() { return mana_; }
    const resource::Energy& mana() const { return mana_; }
    double max_mana() const { return mana_.max_value(); }

    void set_mana(resource::Energy &mana) { mana_ = mana; }
    void set_mana(double mana) { mana_.Set(mana); }
    void set_skill(Controller::SkillSlot slot, skills::Skill *skill) {
		active_skills_[slot] = skill;
	}

  protected:
    /// The owner.
    sprite::WorldObject* owner_;

    /// The active weapons this creature has.
    std::map<Controller::SkillSlot, skills::Skill*> active_skills_;

    /// The mana of this creature. An energy manages reneration.
    resource::Energy mana_;

    /// An aim resource. It's origin points to the creature's position and the destination to the creature's aim.
    skills::usearguments::Aim aim_;

};  // class Caster

}  // namespace component

#endif  // HORUSEYE_COMPONENT_CASTER_H_
