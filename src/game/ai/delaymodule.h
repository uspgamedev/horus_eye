
#ifndef HORUSEYE_GAME_AI_DELAYMODULE_H_
#define HORUSEYE_GAME_AI_DELAYMODULE_H_

#include <functional>
#include <memory>

#include "game/ai.h"
#include "game/ai/aimodule.h"

namespace ai {

class DelayModule : public AIModule {
  public:
    DelayModule(double the_delay, bool the_repeat_flag)
        : delay_(the_delay), repeat_(the_repeat_flag) {}
    ~DelayModule() {}

    void Start();
    AIModule::Status Update(double dt, AIData* data);
    void Finish();

    void set_child(AIModule* child);

  protected:
    std::unique_ptr<AIModule> child_;
    double                    delay_, time_;
    bool                      repeat_;
};

} // namespace ai

#endif // HORUSEYE_GAME_AI_DELAYMODULE_H_

