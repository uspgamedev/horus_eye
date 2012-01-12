#include <vector>
#include "aimodule.h"
#include "sequencemodule.h"

namespace ai {

using namespace std;

void SequenceModule::Start() {
	for (vector<AIModule*>::iterator it = childs_.begin(); it != childs_.end(); ++it) {
        AIModule *child = *it;
		child->Start();
    }
}

AIModule::Status SequenceModule::Update(float dt) {
	unsigned child_index = 0;
	AIModule* child;
	AIModule::Status stat;

	while (child_index < childs_.size() ) {
		child = childs_[child_index];
		stat = child->Update(dt);

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