#include "checkherovisible.h"
#include "game/sprites/creatures/mummy.h"
#include "game/scenes/world.h"
#include "game/sprites/creatures/hero.h"

using namespace ugdk;
using namespace scene;
using namespace sprite;

namespace ai {

void CheckHeroVisible::Start() {
}

AIModule::Status CheckHeroVisible::Update(float dt) {
	sprite::Creature* owner = parent_->get_root()->get_owner();
	
	if (owner->waiting_animation() ) return AIModule::DORMANT;

	sprite::Mummy* mummy = static_cast<sprite::Mummy*>(owner);
	
	if (WORLD()->hero() != NULL && vision_strategy_.IsVisible(owner->world_position())) {
		mummy->set_standing(false);
		mummy->set_saw_hero(true);
		mummy->UpdateDirections(WORLD()->hero()->world_position() );
		mummy->set_last_known_hero_pos( WORLD()->hero()->world_position() );
		return AIModule::ACTIVE;
	}
	return AIModule::DORMANT;
}

void CheckHeroVisible::Finish() {
}

}