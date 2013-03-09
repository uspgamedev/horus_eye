#include <cstdlib>
#include <ugdk/script.h>
#include <ugdk/script/virtualobj.h>
#include "game/ai/ai.h"
#include "game/ai/aimodule.h"
#include "game/ai/aidata.h"

namespace ai {

AI::AI(sprite::WorldObject *owner, ugdk::script::LangWrapper* script_wrapper, const std::string& name) 
    : owner_(owner), name_(name), root_(NULL), state_(CREATED) {
    data_ = new AIData(script_wrapper);
}

AI::~AI() {
    delete data_;
    if (root_)
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
	Start(); /*Due to the state_, this should execute only once.*/
	if (state_ != RUNNING) return;

    data_->Clear();
	AIModule::Status stat;
	if (root_) {
		stat = root_->Update(dt, data_);
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

void AI::set_root(AIModule* root) {
	if (state_ != CREATED) return;

	root_ = root;
	root_->base_ = this;
}

void AI::set_standing(bool standing) {
    ugdk::script::VirtualObj vbool (data_->script_wrapper());
    vbool.set_value(standing);
    data_->SetSharedData("standing", vbool);
}

bool AI::standing() {
    ugdk::script::VirtualObj vstanding = data_->GetSharedData("standing");
    if (vstanding)
        return vstanding.value<bool>();
    return false;
}

bool AI::IsUsingSkillSlot(SkillSlot slot) const {
	if (state_ != RUNNING) return false;
    return data_->IsUsingSkillSlot(slot);
}

const ugdk::math::Vector2D& AI::direction_vector() const { 
    return data_->direction(); 
}

}
