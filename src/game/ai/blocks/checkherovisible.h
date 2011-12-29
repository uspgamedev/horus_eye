#ifndef HORUSEYE_GAME_AI_BLOCK_CHECKHEROVISIBLE_H_
#define HORUSEYE_GAME_AI_BLOCK_CHECKHEROVISIBLE_H_

#include "game/ai/ailogicblock.h"
#include "game/utils/visionstrategy.h"

namespace ai {

class CheckHeroVisible : public AILogicBlock {

public:
	CheckHeroVisible(LogicModule* parent) : AILogicBlock(parent) {}
	~CheckHeroVisible() {}

	virtual void Start();
	virtual AIModule::Status Update(float dt);
	virtual void Finish();

protected:
	utils::VisionStrategy vision_strategy_;
};

}
#endif // CHECKHEROVISIBLE_H_