#include "updatedirections.h"
#include "game/sprites/creatures/mummy.h"

using namespace ugdk;

namespace ai {

void UpdateDirections::Start() {
}

AIModule::Status UpdateDirections::Update(float dt) {
	sprite::Creature* owner = parent_->get_root()->get_owner();
	
	if (owner->waiting_animation() ) return AIModule::DORMANT;

	sprite::Mummy* mummy = static_cast<sprite::Mummy*>(owner);
	
	//mummy->UpdateDirections( strategy_.Calculate(mummy->world_position()) );

	return AIModule::ACTIVE;
}

void UpdateDirections::Finish() {
}

}