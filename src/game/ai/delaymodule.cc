
#include "game/ai/delaymodule.h"

#include <cstdio>

namespace ai {

void DelayModule::Start() {
    if (child_) child_->Start();
}

AIModule::Status DelayModule::Update(double dt, AIData* data) {
    if (child_) {
      time_ += dt;
      if (time_ >= delay_) {
          time_ -= delay_;
          Status stat = child_->Update(dt, data);
          return repeat_ ? stat : AIModule::DONE;
      }
    }
    return AIModule::DORMANT;
}

void DelayModule::Finish() {
    if (child_) child_->Finish();
}

void DelayModule::set_child(AIModule* child) { 
    child_.reset(child);
    child->set_parent(this); 
}

} // namespace ai

