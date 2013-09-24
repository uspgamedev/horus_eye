#ifndef HORUSEYE_GAME_AI_RANDOMMODULE_H_
#define HORUSEYE_GAME_AI_RANDOMMODULE_H_

#include "game/ai/aimodule.h"

#include <vector>
#include <memory>
#include <utility>

namespace ai {

class RandomModule : public AIModule {
  public:
	RandomModule();
	~RandomModule();

	void Start();
	AIModule::Status Update(double dt, AIData* data);
	void Finish();

	void AddChildModule(AIModule* child, double chance);

    // Distributes the same chance to all modules.
	void SetUniformDistribution();

  protected:
    double total_probability_;
    std::vector<std::pair<std::unique_ptr<AIModule>, double> > childs_;
};

} // namespace ai

#endif // HORUSEYE_GAME_AI_RANDOMMODULE_H_
