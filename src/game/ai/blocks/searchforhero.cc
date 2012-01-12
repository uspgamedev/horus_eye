#include "searchforhero.h"
#include "game/sprites/creatures/mummy.h"

using namespace ugdk;

namespace ai {

void SearchForHero::Start() {
}

AIModule::Status SearchForHero::Update(float dt) {
	sprite::Creature* owner = parent_->get_root()->get_owner();
	
	if (owner->waiting_animation() ) return AIModule::DORMANT;

	sprite::Mummy* mummy = static_cast<sprite::Mummy*>(owner);
	
	Vector2D dist = mummy->last_known_hero_pos() - mummy->world_position();

	if (dist.LengthSquared() > 0.5f && mummy->saw_hero()) {
		mummy->UpdateDirections( mummy->last_known_hero_pos() );
		mummy->DoMove(dt);
		return AIModule::ACTIVE;
	}
	mummy->set_saw_hero(false);
	return AIModule::DORMANT;
}

void SearchForHero::Finish() {
}

}