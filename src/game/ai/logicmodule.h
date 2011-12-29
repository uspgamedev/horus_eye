#ifndef HORUSEYE_GAME_AI_LOGICMODULE_H_
#define HORUSEYE_GAME_AI_LOGICMODULE_H_

#include <stdlib.h>
#include "aimodule.h"

namespace ai {

class AILogicBlock;

class LogicModule : public AIModule {

public:
	LogicModule() : AIModule(), child_(NULL), logic_(NULL) {}
	~LogicModule() {}

	virtual void Start();
	virtual AIModule::Status Update(float dt);
	virtual void Finish();

	void SetChildModule(AIModule* child) { child_ = child; child->set_parent(this); }
	void SetLogicObject(AILogicBlock* logic) { logic_ = logic; }

protected:
	AIModule* child_;
	AILogicBlock* logic_;
};

}
#endif // LOGICMODULE_H_