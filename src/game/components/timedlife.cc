#include "timedlife.h"

#include "game/sprites/worldobject.h"

namespace component {

TimedLife::TimedLife(double duration)
    : owner_(nullptr)
    , time_elapsed_(0.0)
    , duration_(duration) {}

TimedLife::~TimedLife() {}

// ============= other stuff

void TimedLife::Update(double dt) {
    time_elapsed_ += dt;
    if (time_elapsed_ >= duration_)
        owner_->Die();
}
    
void TimedLife::OnAdd(sprite::WorldObject* owner) {
    owner_ = owner;
}

}  // namespace component
