#include <cmath>
#include <ugdk/math/vector2D.h>
#include "game/ai/blocks/randommovement.h"
#include "game/sprites/worldobject.h"

#define PI 3.1415926535897931

namespace ai {
namespace blocks {

void RandomMovement::Start() {
	time_left_ = WaitingTime();
}

AIModule::Status RandomMovement::Update(double dt, AIData* data) {
	sprite::WorldObject* owner = parent_->root()->owner();
	
	if (!owner->is_active() ) return AIModule::DORMANT;

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
        data->set_direction( ugdk::Vector2D(cos(dir*PI/4.0),sin(dir*PI/4.0)) );
	}
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
