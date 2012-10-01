#include "game/ai/ai.h"
#include "game/ai/aimodule.h"
#include "game/ai/aidata.h"

namespace ai {

AI::AI(sprite::Creature *owner) : super(owner), root_(NULL), state_(CREATED) {
    data_ = new AIData();
}

AI::~AI() {
    delete data_;
    delete root_;
}

void AI::Start() {
	if (state_ != CREATED) return;

	if (root_) {
		root_->Start();
		state_ = RUNNING;
	}
}

void AI::Update(double dt) {
	if (state_ != RUNNING) return;

    data_->Clear();
	AIModule::Status stat;
	if (root_) {
		stat = root_->Update(dt);
		if (stat == AIModule::DONE) {
			Finish();
		}
	}
    aim_destination_ = data_->aim_destination();
    dir_ = component::Direction::FromWorldVector(data_->direction());
}

void AI::Finish(){
	if (state_ != RUNNING) return;

	if (root_) {
		root_->Finish();
		state_ = FINISHED;
	}
}

void AI::SetRootModule(AIModule* root) {
	if (state_ != CREATED) return;

	root_ = root;
	root_->root_ = this;
}

bool AI::IsUsingSkillSlot(SkillSlot slot) {
	if (state_ != RUNNING) return false;
    return data_->IsUsingSkillSlot(slot);
}

}
