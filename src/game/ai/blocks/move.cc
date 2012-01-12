#include "move.h"
#include "game/sprites/creatures/mummy.h"

using namespace ugdk;

namespace ai {

void Move::Start() {
}

AIModule::Status Move::Update(float dt) {
	sprite::Creature* owner = parent_->get_root()->get_owner();
	
	if (owner->waiting_animation() ) return AIModule::DORMANT;

	owner->DoMove(dt);

	return AIModule::ACTIVE;
}

void Move::Finish() {
}

}