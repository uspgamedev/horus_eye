#ifndef HORUSEYE_GAME_AI_BLOCK_CHECKHEROVISIBLE_H_
#define HORUSEYE_GAME_AI_BLOCK_CHECKHEROVISIBLE_H_

#include "game/ai.h"
#include "game/ai/logicblock.h"
#include "game/utils/visionstrategy.h"

namespace ai {
namespace blocks {

class CheckHeroVisible : public LogicBlock {

public:
	CheckHeroVisible() {}
	~CheckHeroVisible() {}

	virtual void Start();
	virtual AIModule::Status Update(double dt, AIData* data);
	virtual void Finish();

protected:
	utils::VisionStrategy vision_strategy_;
};

}
}
#endif // CHECKHEROVISIBLE_H_
