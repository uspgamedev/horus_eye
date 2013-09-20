#include <vector>
#include "game/ai/aimodule.h"
#include "game/ai/sequencemodule.h"

namespace ai {

SequenceModule::~SequenceModule() {
    for (AIModule* child : childs_)
        delete child;
    childs_.clear();
}

void SequenceModule::Start() {
    for (AIModule* child : childs_)
        child->Start();
}

AIModule::Status SequenceModule::Update(double dt, AIData* data) {
    for(AIModule* child : childs_) {
        AIModule::Status stat = child->Update(dt, data);
        if(stat != AIModule::DORMANT)
            return stat;
    }
    return AIModule::DORMANT;
}

void SequenceModule::Finish() {
    for (AIModule* child : childs_)
        child->Finish();
}

} // namespace ai
