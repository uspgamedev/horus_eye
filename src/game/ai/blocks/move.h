#ifndef HORUSEYE_GAME_AI_BLOCK_MOVE_H_
#define HORUSEYE_GAME_AI_BLOCK_MOVE_H_

#include "game/ai/ailogicblock.h"

namespace ai {

class Move : public AILogicBlock {

public:
	Move(LogicModule* parent) : AILogicBlock(parent) {}
	~Move() {}

	virtual void Start();
	virtual AIModule::Status Update(float dt);
	virtual void Finish();

protected:
	
};

}
#endif // MOVE_H_