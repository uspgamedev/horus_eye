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
	virtual AIModule::Status Update(double dt);
	virtual void Finish();

	void AddChildModule(AIModule* child, double chance);
	void SetUniformDistribution();

  protected:
	std::vector<AIModule*> childs_;
	std::vector<double> probability_distribution_;
};

}
#endif // RANDOMMODULE_H_
