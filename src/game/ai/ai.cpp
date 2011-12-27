#include "ai.h"
#include "aimodule.h"
#include "game/sprites/creatures/creature.h"

namespace ai {

AI::AI(sprite::Creature *owner) : owner_(owner), root_(NULL), state_(CREATED) {
}

AI::~AI() {
}

void AI::Start() {
	if (state_ != CREATED) return;

	if (root_) {
		root_->Start();
		state_ = RUNNING;
	}
}

void AI::Update(float dt) {
	if (state_ != RUNNING) return;

	AIModule::Status stat;
	if (root_) {
		stat = root_->Update(dt);
		if (stat == AIModule::Status::DONE) {
			Finish();
		}
	}
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
}

}