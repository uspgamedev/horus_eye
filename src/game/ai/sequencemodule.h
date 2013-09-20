#ifndef HORUSEYE_GAME_AI_SEQUENCEMODULE_H_
#define HORUSEYE_GAME_AI_SEQUENCEMODULE_H_

#include "game/ai/aimodule.h"
#include <vector>

namespace ai {

class SequenceModule : public AIModule {
  public:
	SequenceModule() {}
	~SequenceModule();

	void Start();

    /** In sequence, calls the Update of each child, stopping at the first
        that returns non-DORMANT. */
	AIModule::Status Update(double dt, AIData* data);

	void Finish();

	void AddChildModule(AIModule* child) { childs_.push_back(child); child->set_parent(this); }

  protected:
	std::vector<AIModule*> childs_;
};

} // namespace ai

#endif // HORUSEYE_GAME_AI_SEQUENCEMODULE_H_
