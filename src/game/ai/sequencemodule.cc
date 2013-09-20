#include <vector>
#include "game/ai/aimodule.h"
#include "game/ai/sequencemodule.h"

namespace ai {

SequenceModule::~SequenceModule() {}

void SequenceModule::Start() {
    for (std::unique_ptr<AIModule>& child : childs_)
        child->Start();
}

AIModule::Status SequenceModule::Update(double dt, AIData* data) {
    for(std::unique_ptr<AIModule>& child : childs_) {
        AIModule::Status stat = child->Update(dt, data);
        if(stat != AIModule::DORMANT)
            return stat;
    }
    return AIModule::DORMANT;
}

void SequenceModule::Finish() {
    for (std::unique_ptr<AIModule>& child : childs_)
        child->Finish();
}
	
void SequenceModule::AddChildModule(AIModule* child) {
    childs_.emplace_back(child);
    child->set_parent(this);
}

} // namespace ai
