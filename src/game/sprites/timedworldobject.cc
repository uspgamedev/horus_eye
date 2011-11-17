#include "timedworldobject.h"

namespace sprite {

 TimedWorldObject::TimedWorldObject(float duration)
    : WorldObject(),
      timed_life_(new ugdk::TimeAccumulator((int)(duration * 1000))) {
}

TimedWorldObject::~TimedWorldObject() {
    delete timed_life_;
}

void TimedWorldObject::Update(float dt) {
    WorldObject::Update(dt);
	if(timed_life_->Expired())
		this->status_ = WorldObject::STATUS_DEAD;
}

}  // namespace sprite
