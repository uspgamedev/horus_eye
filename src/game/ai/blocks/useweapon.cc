#include "useweapon.h"
#include "game/sprites/creatures/mummy.h"
#include "game/skills/skill.h"

using namespace ugdk;

namespace ai {

void UseWeapon::Start() {
}

AIModule::Status UseWeapon::Update(float dt) {
	sprite::Creature* owner = parent_->get_root()->get_owner();
	
	if (owner->waiting_animation() ) return AIModule::DORMANT;

	sprite::Mummy* mummy = static_cast<sprite::Mummy*>(owner);
	
	if(weapon_->Avaiable() && weapon_->IsValidUse()) {
		weapon_->Use();
        mummy->StartAttack(NULL);
		return AIModule::ACTIVE;
    }
	return AIModule::DORMANT;
}

void UseWeapon::Finish() {
}

}