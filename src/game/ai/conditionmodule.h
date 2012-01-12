#ifndef HORUSEYE_GAME_AI_CONDITIONMODULE_H_
#define HORUSEYE_GAME_AI_CONDITIONMODULE_H_

#include "logicmodule.h"

namespace ai {

class ConditionModule : public LogicModule {

public:
	ConditionModule();
	~ConditionModule();

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