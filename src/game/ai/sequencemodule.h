#ifndef HORUSEYE_GAME_AI_SEQUENCEMODULE_H_
#define HORUSEYE_GAME_AI_SEQUENCEMODULE_H_

#include <vector>
#include "aimodule.h"

namespace ai {

class SequenceModule : public AIModule {
  public:
	SequenceModule() : AIModule() {}
	~SequenceModule() {}

	virtual void Start();
	virtual AIModule::Status Update(float dt);
	virtual void Finish();

	void AddChildModule(AIModule* child) { childs_.push_back(child); child->set_parent(this); }

  protected:
	std::vector<AIModule*> childs_;
};

}
#endif // SEQUENCEMODULE_H_