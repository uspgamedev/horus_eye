#include "timedworldobject.h"

#define SECONDS_TO_MILISECONDS(sec) (int)((sec) * 1000)

namespace sprite {

TimedWorldObject::TimedWorldObject(float duration)
    : WorldObject(),
      timed_life_(new ugdk::TimeAccumulator(SECONDS_TO_MILISECONDS(duration))) 
    {}

TimedWorldObject::~TimedWorldObject() {
    delete timed_life_;
}

void TimedWorldObject::Update(float dt) {
    WorldObject::Update(dt);
	if(timed_life_->Expired() && is_active()) Die();
}

}  // namespace sprite
