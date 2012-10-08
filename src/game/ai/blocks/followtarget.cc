#include "game/ai/blocks/followtarget.h"
#include "game/ai/aidata.h"
#include "game/ai/ai.h"
#include "game/sprites/worldobject.h"
#include "game/context.h"
#include "game/components/animation.h"
#include <ugdk/script/virtualobj.h>
#include <string>

namespace ai {
namespace blocks {

void FollowTarget::Start() {
}

AIModule::Status FollowTarget::Update(double dt, AIData* data) {
	sprite::WorldObject* owner = parent_->base()->owner();
	
	if (owner->animation()->is_uninterrutible() ) return AIModule::DORMANT;
    if (owner->is_active() ) return AIModule::DORMANT;

    if (target_ == NULL && target_tag_.size() > 0) {
        target_ = context::WorldObjectByTag(target_tag_);
    }

	if (target_ != NULL && vision_strategy_.IsVisible(owner, target_->world_position())) {
        ugdk::script::VirtualObj vfalse (data->script_wrapper());
        vfalse.set_value(false);
        data->SetSharedData("standing", vfalse);        

		search_target_ = true;
		last_known_target_pos_ = target_->world_position();

		data->set_direction(target_->world_position() );
		return AIModule::ACTIVE;
	}
    else if (search_target_) {
	    ugdk::Vector2D dist = last_known_target_pos_ - owner->world_position();

	    if (dist.LengthSquared() > 0.5) {
    		data->set_direction( last_known_target_pos_ );
		    return AIModule::ACTIVE;
	    }
	    search_target_ = false;
    }

	return AIModule::DORMANT;
}

void FollowTarget::Finish() {
}

}
}
