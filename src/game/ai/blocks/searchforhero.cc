#include "game/ai/blocks/searchforhero.h"
#include "game/ai/aidata.h"
#include "game/ai/ai.h"
#include "game/components/animation.h"
#include <ugdk/math/vector2D.h>

namespace ai {
namespace blocks {

void SearchForHero::Start() {
}

AIModule::Status SearchForHero::Update(double dt, AIData* data) {
	sprite::WorldObject* owner = parent_->base()->owner();
	
    if (owner->animation()->is_uninterrutible() ) return AIModule::DORMANT;
    if (owner->is_active() ) return AIModule::DORMANT;

	/*ugdk::Vector2D dist = mummy->last_known_hero_pos() - mummy->world_position();

	if (dist.LengthSquared() > 0.5f && mummy->saw_hero()) {
		mummy->UpdateDirections( mummy->last_known_hero_pos() );
		mummy->DoMove(dt);
		return AIModule::ACTIVE;
	}
	mummy->set_saw_hero(false);*/
	return AIModule::DORMANT;
}

void SearchForHero::Finish() {
}

}
}
