#include "game/ai/conditionmodule.h"

namespace ai {

ConditionModule::~ConditionModule() {
    delete child_;
}

void ConditionModule::Start() {
    if (child_) child_->Start();
}

AIModule::Status ConditionModule::Update(double dt, AIData* data) {
    bool ok = eval_func_(this, dt, data);
    if (child_ && ok)
        return child_->Update(dt, data);
    return AIModule::DORMANT;
}

void ConditionModule::Finish() {
    if (child_) child_->Finish();
}

void ConditionModule::set_child(AIModule* child) { 
    child_ = child; 
    child->set_parent(this); 
}


}
