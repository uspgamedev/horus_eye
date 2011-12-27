#include "aimodule.h"
#include "logicmodule.h"
#include "ailogicblock.h"

namespace ai {

void LogicModule::Start() {
	if (child_)
		child_->Start();
	if (logic_)
		logic_->Start();
}

AIModule::Status LogicModule::Update(float dt) {
	if (!logic_) return Status::DONE;

	AIModule::Status stat = logic_->Update(dt);
	if (child_ && stat == Status::ACTIVE)
		stat = child_->Update(dt);
	return stat;
}

void LogicModule::Finish(){
	if (child_)
		child_->Finish();
	if (logic_)
		logic_->Finish();
}

}