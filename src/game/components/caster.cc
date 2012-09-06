#include "game/components/caster.h"

#include "game/sprites/worldobject.h"
#include "game/skills/skill.h"

using sprite::WorldObject;
using skills::Skill;

#define PI 3.1415926535897932384626433832795

namespace component {

Caster::Caster(WorldObject* owner, const resource::Energy& mana, int block_count, const skills::usearguments::Aim& aim)
    : owner_(owner), mana_(mana), mana_blocks_(mana_, block_count),
      aim_(aim) {}

Caster::Caster(sprite::WorldObject* owner, const resource::Energy& mana)
	: owner_(owner), mana_(mana), mana_blocks_(mana_, 1),
	  aim_(owner->world_position(), owner->controller()->aim_destination()) {}

Caster::~Caster() {
    std::map<Controller::SkillSlot, skills::Skill*>::iterator it;
    for(it = active_skills_.begin(); it != active_skills_.end(); ++it)
        delete it->second;
}

// ============= other stuff

void Caster::Update(double dt) {
    mana_.Update(dt);
}

void Caster::AddSkill(int slot, skills::Skill* skill) {
    if (!skills_.count(slot)) skills_[slot] = skill;
}

void Caster::EquipSkill(int id, Controller::SkillSlot skill_slot) {
	active_skills_[skill_slot] = skills_[id];
}

bool Caster::CastSkill(Controller::SkillSlot slot) {
	Skill* skill = active_skills_[slot];
	if(skill && skill->Available()) {
		skill->Use();
		return true;
	}
	return false;
}

}  // namespace sprite
