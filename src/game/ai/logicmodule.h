#ifndef HORUSEYE_GAME_AI_LOGICMODULE_H_
#define HORUSEYE_GAME_AI_LOGICMODULE_H_

#include "game/ai.h"
#include "game/ai/aimodule.h"

namespace ai {

class LogicModule : public AIModule {

public:
	LogicModule() : child_(0), logic_(0) {}
	~LogicModule();

	void Start();
	AIModule::Status Update(double dt, AIData* data);
	void Finish();

	void SetChildModule(AIModule* child) { child_ = child; child->set_parent(this); }
	void SetLogicObject(AILogicBlock* logic) { logic_ = logic; }

protected:
	AIModule* child_;
	AILogicBlock* logic_;
};

}
#endif // LOGICMODULE_H_
