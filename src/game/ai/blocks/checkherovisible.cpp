#include "checkherovisible.h"
#include "game/sprites/creatures/mummy.h"

using namespace ugdk;

namespace ai {

void CheckHeroVisible::Start() {
}

AIModule::Status CheckHeroVisible::Update(float dt) {
	sprite::Creature* owner = parent_->get_root()->get_owner();
	
	if (owner->waiting_animation() ) return AIModule::Status::DORMANT;

	sprite::Mummy* mummy = static_cast<sprite::Mummy*>(owner);
	
	if (vision_strategy_.IsVisible(owner->world_position())) {\
		mummy->set_standing(false);
		return AIModule::Status::ACTIVE;
	}	
	return AIModule::Status::DORMANT;
}

void CheckHeroVisible::Finish() {
}

}