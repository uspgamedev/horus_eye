#include "game/ai/blocks/checktargetvisible.h"
#include "game/ai/aidata.h"
#include "game/ai/ai.h"
#include "game/sprites/worldobject.h"
#include "game/context.h"
#include "game/components/animation.h"
#include <string>

namespace ai {
namespace blocks {

void CheckTargetVisible::Start() {
}

AIModule::Status CheckTargetVisible::Update(double dt, AIData* data) {
	sprite::WorldObject* owner = parent_->base()->owner();
	
	if (owner->animation()->is_uninterrutible() ) return AIModule::DORMANT;
    if (owner->is_active() ) return AIModule::DORMANT;

    if (target_ == NULL && target_tag_.size() > 0) {
        target_ = context::WorldObjectByTag(target_tag_);
    }

	if (target_ != NULL && vision_strategy_.IsVisible(owner, target_->world_position())) {
		/*mummy->set_standing(false);
		mummy->set_saw_hero(true);
		mummy->set_last_known_hero_pos( WORLD()->hero()->world_position() );*/
		data->set_direction(target_->world_position() );
		return AIModule::ACTIVE;
	}
	return AIModule::DORMANT;
}

void CheckTargetVisible::Finish() {
}

}
}
