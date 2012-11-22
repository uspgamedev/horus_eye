#include "game/ai/blocks/followtarget.h"
#include "game/ai/aidata.h"
#include "game/ai/ai.h"
#include "game/sprites/worldobject.h"
#include "game/components/animation.h"
#include <ugdk/script/virtualobj.h>
#include <string>
#include <cstdio>

namespace ai {
namespace blocks {

void FollowTarget::Start() {
}

AIModule::Status FollowTarget::Update(double dt, AIData* data) {
	sprite::WorldObject* owner = parent_->base()->owner();
	
    //if (!owner->component<component::Animation>()->CanInterrupt() ) return AIModule::DORMANT;
    if (owner->dead()) return AIModule::DORMANT;

    if (detector_identifier_.size() <= 0) {
        return AIModule::DORMANT;
    }

    sprite::WorldObject* target = NULL;
    ugdk::script::VirtualObj vtarget = data->GetSharedData(detector_identifier_+"_target");
    if (vtarget) {
        target = vtarget.value<sprite::WorldObject*>(true);
    }

	if (target != NULL) {// && vision_strategy_.IsVisible(owner, target->world_position())) {
        parent_->base()->set_standing(false);  

		search_target_ = true;
		last_known_target_pos_ = target->world_position();

		data->set_direction( (target->world_position() - owner->world_position()).Normalize() );
        data->set_aim_destination( target->world_position() );
		return AIModule::ACTIVE;
	}
    else if (search_target_) {
	    ugdk::Vector2D dist = last_known_target_pos_ - owner->world_position();

        /*Check if we arrived at the last_known_target_pos_ (or really close to it) */
	    if (dist.LengthSquared() > 0.5) {
    		data->set_direction( dist.Normalize() );
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
