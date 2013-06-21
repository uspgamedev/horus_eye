#ifndef HORUSEYE_GAME_AI_CONDITIONMODULE_H_
#define HORUSEYE_GAME_AI_CONDITIONMODULE_H_

#include <functional>

#include "game/ai.h"
#include "game/ai/aimodule.h"

namespace ai {

typedef std::function<bool (ConditionModule*, double, AIData*)> ConditionEvalFunc;

class ConditionModule : public AIModule {

public:
	ConditionModule(ConditionEvalFunc eval_func) : eval_func_(eval_func) {}
	~ConditionModule();

	void Start();
	AIModule::Status Update(double dt, AIData* data);
	void Finish();

	void set_child(AIModule* child);

  protected:
	AIModule* child_;

    ConditionEvalFunc eval_func_;


	/*ConditionModule (class, derived from LogicModule):
	-EvalFunc: evaluation function pointer

	-override Start(): calls start from child
	-override Update(delta_t): do EvalFunc(), which returns a status code, based on data from Logic (condition logic). 
								If Status is DORMANT|DONE, return that, 
								if is ACTIVE do child.Update() and return the child status.
	-override Finish(): calls finish from child*/
};

}
#endif // CONDITIONMODULE_H_
