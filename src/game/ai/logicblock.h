#ifndef HORUSEYE_GAME_AI_AILOGICBLOCK_H_
#define HORUSEYE_GAME_AI_AILOGICBLOCK_H_

#include "game/ai.h"

namespace ai {

class LogicBlock {

  public:
	~LogicBlock() {}

	virtual void Start() {}
	virtual AIModule::Status Update(double dt, AIData* data) = 0;
	virtual void Finish() {}

	LogicModule* parent() { return parent_; }

  protected:
	LogicModule* parent_;

	LogicBlock() : parent_(0) { }

    void set_parent(LogicModule* parent) { parent_ = parent; }

    friend class LogicModule;
};

}
#endif // AILOGICBLOCK_H_
