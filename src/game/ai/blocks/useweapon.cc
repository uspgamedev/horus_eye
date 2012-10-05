#include "game/ai/blocks/useweapon.h"
#include "game/ai/aidata.h"
#include "game/ai/ai.h"
#include "game/sprites/worldobject.h"
#include "game/skills/skill.h"
#include "game/components/animation.h"
#include "game/components/caster.h"

namespace ai {
namespace blocks {

void UseWeapon::Start() {
}

AIModule::Status UseWeapon::Update(double dt, AIData* data) {
	sprite::WorldObject* owner = parent_->base()->owner();
	
    if (owner->animation()->is_uninterrutible() ) return AIModule::DORMANT;
    if (owner->is_active() ) return AIModule::DORMANT;

    component::Caster* caster = owner->caster();
    skills::Skill* skill = caster->SkillAt(slot_);
    if (!skill) return AIModule::DORMANT;

	if(skill->Available(caster) && skill->IsValidUse(caster)) {
		data->AddUsingSkillSlot(slot_);
		return AIModule::ACTIVE;
    }
	return AIModule::DORMANT;
}

void UseWeapon::Finish() {
}

}
}
