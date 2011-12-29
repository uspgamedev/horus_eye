#ifndef HORUSEYE_GAME_AI_BLOCK_RANDOMMOVEMENT_H_
#define HORUSEYE_GAME_AI_BLOCK_RANDOMMOVEMENT_H_

#include "game/ai/ailogicblock.h"

namespace ai {

class RandomMovement : public AILogicBlock {

public:
	RandomMovement(LogicModule* parent, float time_to_change_direction) : 
					AILogicBlock(parent), time_to_change_(time_to_change_direction) {}
	~RandomMovement() {}

	virtual void Start();
	virtual AIModule::Status Update(float dt);
	virtual void Finish();

protected:
	float time_to_change_;
	float time_left_;

	// Devolve um tempo ~exp(time_to_change_)
	float WaitingTime();
};

}
#endif // RANDOMMOVEMENT_H_