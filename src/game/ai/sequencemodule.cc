#include <vector>
#include "game/ai/aimodule.h"
#include "game/ai/sequencemodule.h"

namespace ai {

using std::vector;

SequenceModule::~SequenceModule() {
    for (vector<AIModule*>::iterator it = childs_.begin(); it != childs_.end(); ++it) {
        AIModule *child = *it;
		delete child;
    }
    childs_.clear();
}

void SequenceModule::Start() {
	for (vector<AIModule*>::iterator it = childs_.begin(); it != childs_.end(); ++it) {
        AIModule *child = *it;
		child->Start();
    }
}

AIModule::Status SequenceModule::Update(double dt, AIData* data) {
	unsigned child_index = 0;
	AIModule* child;
	AIModule::Status stat;

	while (child_index < childs_.size() ) {
		child = childs_[child_index];
		stat = child->Update(dt, data);

		if (stat == AIModule::DORMANT)
			child_index++;
		else
			break;
	}

	return stat;
}

void SequenceModule::Finish(){
	for (vector<AIModule*>::iterator it = childs_.begin(); it != childs_.end(); ++it) {
        AIModule *child = *it;
		child->Finish();
    }
}

}
