#include <cmath>
#include <cstdlib>
#include <ugdk/math/vector2D.h>
#include <ugdk/script/virtualobj.h>
#include "game/ai/blocks/randommovement.h"
#include "game/ai/ai.h"
#include "game/ai/aidata.h"
#include "game/sprites/worldobject.h"
#include "game/components/animation.h"

#define PI 3.1415926535897931

namespace ai {
namespace blocks {

void RandomMovement::Start() {
	time_left_ = WaitingTime();
    int dir = rand()%8;
    direction_ = ugdk::Vector2D(cos(dir*PI/4.0),sin(dir*PI/4.0));
}

AIModule::Status RandomMovement::Update(double dt, AIData* data) {
	sprite::WorldObject* owner = parent_->base()->owner();
	
    if (owner->component<component::Animation>()->is_uninterrutible() ) return AIModule::DORMANT;
	if (!owner->is_active() ) return AIModule::DORMANT;

    ugdk::script::VirtualObj vstanding = data->GetSharedData("standing");
    if (vstanding) {
        bool standing = vstanding.value<bool>();
        if (standing)   return AIModule::DORMANT;
    }

	time_left_ -= dt;
	if (time_left_ < 0) {
		time_left_ = WaitingTime();

		int dir = rand()%8;

        //if (dir < 3) animation_direction_ += Animation_::UP;
        //if (dir >= 2 && dir < 5) animation_direction_ += Animation_::LEFT;
        //if (dir >= 4 && dir < 7) animation_direction_ += Animation_::DOWN;
        //if (dir >= 6 || dir == 0) animation_direction_ += Animation_::RIGHT;

        //last_direction_ = walking_direction_ = Vector2D(cos(dir*PI/4.0f),sin(dir*PI/4.0f));
		//last_standing_animation_ = (standing_animations_[animation_direction_]);
        direction_ = ugdk::Vector2D(cos(dir*PI/4.0),sin(dir*PI/4.0));
    }
    data->set_direction( direction_ );
	return AIModule::ACTIVE;
}

void RandomMovement::Finish() {
}

// Devolve um tempo ~exp(time_to_change_)
double RandomMovement::WaitingTime() {
    return (1.0*-log(1.0*rand()/RAND_MAX)/time_to_change_);
}

}
}
