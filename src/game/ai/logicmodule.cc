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

AIModule::Status LogicModule::Update(double dt) {
	if (!logic_) return DONE;

	AIModule::Status stat = logic_->Update(dt);
	if (child_ && stat == ACTIVE)
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
