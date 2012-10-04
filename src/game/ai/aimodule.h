#ifndef HORUSEYE_GAME_AI_AIMODULE_H_
#define HORUSEYE_GAME_AI_AIMODULE_H_

#include "game/ai.h"

namespace ai {

class AIModule {
  public:

	// Define the status a AIModule can return (mostly in Update, during execution), to manage the AI execution flow. 
	enum Status { ACTIVE, DORMANT, DONE };

	virtual ~AIModule() {}

	virtual void Start() {}
	virtual Status Update(double dt, AIData* data) = 0;
	virtual void Finish() {}

	AI* root() { return root_; }

	void set_parent(AIModule* parent) { parent_ = parent; root_ = parent->get_root(); }
	AIModule* parent() { return parent_; }

  protected:
	AI* root_;
	AIModule* parent_;

	AIModule() : root_(0), parent_(0) {}

	friend class AI;
};

}
#endif // AIMODULE_H_
