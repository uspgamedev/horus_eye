#include "game/ai/aimodule.h"
#include "game/ai/logicmodule.h"
#include "game/ai/logicblock.h"

namespace ai {

LogicModule::~LogicModule() {
    delete logic_;
    delete child_;
}

void LogicModule::Start() {
	if (child_)
		child_->Start();
	if (logic_)
		logic_->Start();
}

AIModule::Status LogicModule::Update(double dt, AIData* data) {
	if (!logic_) return DONE;

	AIModule::Status stat = logic_->Update(dt, data);
	if (child_ && stat == ACTIVE)
		stat = child_->Update(dt, data);
	return stat;
}

void LogicModule::Finish(){
	if (child_)
		child_->Finish();
	if (logic_)
		logic_->Finish();
}

void LogicModule::set_child(AIModule* child) { 
    child_ = child; 
    child->set_parent(this); 
}

void LogicModule::set_logic(LogicBlock* logic) {
    logic_ = logic;
    logic_->set_parent(this);
}

}
