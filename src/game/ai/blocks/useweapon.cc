#include "game/ai/blocks/useweapon.h"
#include "game/ai/aidata.h"
#include "game/sprites/worldobject.h"
#include "game/skills/skill.h"


namespace ai {
namespace blocks {

void UseWeapon::Start() {
}

AIModule::Status UseWeapon::Update(double dt, AIData* data) {
	sprite::WorldObject* owner = parent_->root()->owner();
	
    if (owner->animation()->is_uninterrutible() ) return AIModule::DORMANT;
    if (owner->is_active() ) return AIModule::DORMANT;

    skills::Skill* skill = owner->caster()->SkillAt(slot_);
    if (!skill) return AIModule::DORMANT;

	if(skill->Available() && skill->IsValidUse()) {
		data->AddUsingSkillSlot(slot_);
		return AIModule::ACTIVE;
    }
	return AIModule::DORMANT;
}

void UseWeapon::Finish() {
}

}
}
