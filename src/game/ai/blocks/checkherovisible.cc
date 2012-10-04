#include "game/ai/blocks/checkherovisible.h"
#include "game/ai/aidata.h"
#include "game/sprites/worldobject.h"
#include "game/scenes/world.h"

namespace ai {
namespace blocks {

void CheckHeroVisible::Start() {
}

AIModule::Status CheckHeroVisible::Update(double dt, AIData* data) {
	sprite::WorldObject* owner = parent_->root()->owner();
	
	if (owner->animation()->is_uninterrutible() ) return AIModule::DORMANT;

	if (WORLD()->hero() != NULL && vision_strategy_.IsVisible(owner->world_position())) {
		/*mummy->set_standing(false);
		mummy->set_saw_hero(true);
		mummy->set_last_known_hero_pos( WORLD()->hero()->world_position() );*/
		mummy->UpdateDirections(WORLD()->hero()->world_position() );
		return AIModule::ACTIVE;
	}
	return AIModule::DORMANT;
}

void CheckHeroVisible::Finish() {
}

}
}
