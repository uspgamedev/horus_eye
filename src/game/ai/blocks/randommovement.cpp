#include <cmath>
#include <ugdk/math/vector2D.h>
#include "randommovement.h"
#include "game/sprites/creatures/mummy.h"

using namespace ugdk;

namespace ai {

void RandomMovement::Start() {
	time_left_ = WaitingTime();
}

AIModule::Status RandomMovement::Update(float dt) {
	sprite::Creature* owner = parent_->get_root()->get_owner();
	
	if (owner->waiting_animation() ) return AIModule::Status::DORMANT;

	sprite::Mummy* mummy = static_cast<sprite::Mummy*>(owner);
	if (mummy && mummy->get_standing())	return AIModule::Status::DORMANT;

	time_left_ -= dt;
	if (time_left_ < 0) {
		time_left_ = WaitingTime();

		int dir = rand()%8;
		float PI = acos(-1.0f);
		
        //if (dir < 3) animation_direction_ += Animation_::UP;
        //if (dir >= 2 && dir < 5) animation_direction_ += Animation_::LEFT;
        //if (dir >= 4 && dir < 7) animation_direction_ += Animation_::DOWN;
        //if (dir >= 6 || dir == 0) animation_direction_ += Animation_::RIGHT;

        //last_direction_ = walking_direction_ = Vector2D(cos(dir*PI/4.0f),sin(dir*PI/4.0f));
		//last_standing_animation_ = (standing_animations_[animation_direction_]);
		owner->SetDirection( Vector2D(cos(dir*PI/4.0f),sin(dir*PI/4.0f)) );
	}
	owner->DoMove(dt);
	return AIModule::Status::ACTIVE;
}

void RandomMovement::Finish() {
}

// Devolve um tempo ~exp(time_to_change_)
float RandomMovement::WaitingTime() {
    return (1.0f*-log(1.0f*rand()/RAND_MAX)/time_to_change_);
}

}