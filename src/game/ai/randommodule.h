#ifndef HORUSEYE_GAME_AI_RANDOMMODULE_H_
#define HORUSEYE_GAME_AI_RANDOMMODULE_H_

#include <vector>
#include "game/ai/aimodule.h"

namespace ai {

class RandomModule : public AIModule {
  public:
	RandomModule() {}
	~RandomModule();

	void Start();
	AIModule::Status Update(double dt, AIData* data);
	void Finish();

	void AddChildModule(AIModule* child, double chance);
	void SetUniformDistribution();

  protected:
	std::vector<AIModule*> childs_;
	std::vector<double> probability_distribution_;
};

}
#endif // RANDOMMODULE_H_
