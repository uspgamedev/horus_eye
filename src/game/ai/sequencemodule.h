#ifndef HORUSEYE_GAME_AI_SEQUENCEMODULE_H_
#define HORUSEYE_GAME_AI_SEQUENCEMODULE_H_

#include <vector>
#include "game/ai/aimodule.h"

namespace ai {

class SequenceModule : public AIModule {
  public:
	SequenceModule() {}
	~SequenceModule();

	void Start();
	AIModule::Status Update(double dt, AIData* data);
	void Finish();

	void AddChildModule(AIModule* child) { childs_.push_back(child); child->set_parent(this); }

  protected:
	std::vector<AIModule*> childs_;
};

}
#endif // SEQUENCEMODULE_H_
