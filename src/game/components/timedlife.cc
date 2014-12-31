#include "timedlife.h"

#include "game/sprites/worldobject.h"
#include "game/components/damageable.h"

namespace component {

TimedLife::TimedLife(double duration)
    : owner_(nullptr)
    , time_elapsed_(0.0)
    , duration_(duration) {}

TimedLife::~TimedLife() {}

// ============= other stuff

void TimedLife::Update(double dt) {
    time_elapsed_ += dt;
    if (time_elapsed_ >= duration_) {
        if (auto damageable = owner_->damageable())
            damageable->Die();
        else
            owner_->Remove();
    }
}
    
void TimedLife::OnAdd(sprite::WObjRawPtr owner) {
    owner_ = owner;
}

}  // namespace component
