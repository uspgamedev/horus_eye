#ifndef HORUSEYE_GAME_AI_AILOGICBLOCK_H_
#define HORUSEYE_GAME_AI_AILOGICBLOCK_H_

#include "logicmodule.h"

namespace ai {

class AILogicBlock {

  public:
	~AILogicBlock() {}

	virtual void Start() {}
	virtual AIModule::Status Update(float dt) { return AIModule::DONE; }
	virtual void Finish() {}

	LogicModule* get_parent() { return parent_; }

  protected:
	LogicModule* parent_;

	AILogicBlock(LogicModule* parent) : parent_(parent) { parent_->SetLogicObject(this); }
};

}
#endif // AILOGICBLOCK_H_