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

	AI* base() const { return base_; }

	AIModule* parent() const { return parent_; }
	void set_parent(AIModule* parent) { 
        parent_ = parent;
        base_ = parent->base();
    }

  private:
	AI* base_;
	AIModule* parent_;

  protected:
	AIModule() : base_(nullptr), parent_(nullptr) {}


	friend class AI;
};

}
#endif // AIMODULE_H_
