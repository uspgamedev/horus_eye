#ifndef HORUSEYE_GAME_AI_AIMODULE_H_
#define HORUSEYE_GAME_AI_AIMODULE_H_

#include <stdlib.h>

namespace ai {

class AI;

class AIModule {

  public:

	// Define the status a AIModule can return (mostly in Update, during execution), to manage the AI execution flow. 
	enum Status { ACTIVE, DORMANT, DONE };

	~AIModule() {}

	virtual void Start() {}
	virtual Status Update(float dt) { return DONE; }
	virtual void Finish() {}

	AI* get_root() { return root_; }

	void set_parent(AIModule* parent) { parent_ = parent; root_ = parent->get_root(); }
	AIModule* get_parent() { return parent_; }

  protected:
	AI* root_;
	AIModule* parent_;

	AIModule() : root_(NULL), parent_(NULL) {}

	friend class AI;
};

}
#endif // AIMODULE_H_