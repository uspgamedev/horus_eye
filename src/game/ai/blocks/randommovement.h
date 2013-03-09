#ifndef HORUSEYE_GAME_AI_BLOCK_RANDOMMOVEMENT_H_
#define HORUSEYE_GAME_AI_BLOCK_RANDOMMOVEMENT_H_

#include "game/ai.h"
#include "game/ai/logicblock.h"
#include <ugdk/math/vector2D.h>

namespace ai {
namespace blocks {

class RandomMovement : public LogicBlock {

public:
	RandomMovement(double time_to_change_direction) : 
					time_to_change_(time_to_change_direction) {}
	~RandomMovement() {}

	virtual void Start();
	virtual AIModule::Status Update(double dt, AIData* data);
	virtual void Finish();

protected:
	double time_to_change_;
	double time_left_;
    ugdk::math::Vector2D direction_;

	// Devolve um tempo ~exp(time_to_change_)
	double WaitingTime();
};

}
}
#endif // RANDOMMOVEMENT_H_
