#include "game/ai/aimodule.h"
#include "game/ai/randommodule.h"

namespace ai {

using std::vector;

RandomModule::~RandomModule() {
    for (vector<AIModule*>::iterator it = childs_.begin(); it != childs_.end(); ++it) {
        AIModule *child = *it;
		delete child;
    }
    childs_.clear();
}

void RandomModule::Start() {
	for (vector<AIModule*>::iterator it = childs_.begin(); it != childs_.end(); ++it) {
        AIModule *child = *it;
		child->Start();
    }
}

AIModule::Status RandomModule::Update(double dt, AIData* data) {
	double choice = 100.0 * rand() / RAND_MAX;
	double accumulated_count = 0.0;
	for (int i = 0; i < (int)childs_.size(); i++) {
		accumulated_count += probability_distribution_[i];
		if (choice < accumulated_count) {
			return childs_[i]->Update(dt, data);
		}
    }

	//We don't have any child or we couldn't select a child module within our probability distribution...
	//the "couldn't select" part normally means the probability distribution is wrong...
	//REMEMBER: the sum of all elements of the this->probability_distribution_ vector SHOULD BE 100.0 (even tho this class can be easily
	//			modified so that the maximum/sum value is variable =P)
	return DONE;
}

void RandomModule::Finish(){
	for (vector<AIModule*>::iterator it = childs_.begin(); it != childs_.end(); ++it) {
        AIModule *child = *it;
		child->Finish();
    }
}

void RandomModule::AddChildModule(AIModule* child, double chance) {
	childs_.push_back(child);
	probability_distribution_.push_back(chance);
	child->set_parent(this);
}

void RandomModule::SetUniformDistribution() {
	probability_distribution_.clear();
	double uniform_prob = 0.0;
	if (childs_.size() > 0)
		uniform_prob = 100.0 / childs_.size();
	for (int i = 0; i < (int)childs_.size(); i++) {
		probability_distribution_.push_back(uniform_prob);
    }
}

}
