#ifndef HORUSEYE_GAME_AI_RANDOMMODULE_H_
#define HORUSEYE_GAME_AI_RANDOMMODULE_H_

#include <vector>
#include "aimodule.h"

namespace ai {

class RandomModule : public AIModule {
  public:
	RandomModule() : AIModule() {}
	~RandomModule() {}

	virtual void Start();
	virtual AIModule::Status Update(float dt);
	virtual void Finish();

	void AddChildModule(AIModule* child) { childs_.push_back(child); }

  protected:
	std::vector<AIModule*> childs_;
};

}
#endif // RANDOMMODULE_H_