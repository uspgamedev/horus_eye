#ifndef HORUSEYE_GAME_AI_BLOCK_UPDATEDIRECTIONS_H_
#define HORUSEYE_GAME_AI_BLOCK_UPDATEDIRECTIONS_H_

#include "game/ai/ailogicblock.h"
#include "game/utils/visionstrategy.h"

namespace ai {

class UpdateDirections : public AILogicBlock {

public:
	UpdateDirections(LogicModule* parent) : AILogicBlock(parent) {}
	~UpdateDirections() {}

	virtual void Start();
	virtual AIModule::Status Update(float dt);
	virtual void Finish();

protected:
	utils::VisionStrategy strategy_;
};

}
#endif // UPDATEDIRECTIONS_H_