#include "aimodule.h"
#include "randommodule.h"

namespace ai {

using namespace std;

void RandomModule::Start() {
	for (vector<AIModule*>::iterator it = childs_.begin(); it != childs_.end(); ++it) {
        AIModule *child = *it;
		child->Start();
    }
}

AIModule::Status RandomModule::Update(float dt) {
	if (childs_.size() <= 0) return AIModule::Status::DONE;

	unsigned child_index = rand() % childs_.size();
	return childs_[child_index]->Update(dt);
}

void RandomModule::Finish(){
	for (vector<AIModule*>::iterator it = childs_.begin(); it != childs_.end(); ++it) {
        AIModule *child = *it;
		child->Finish();
    }
}

}