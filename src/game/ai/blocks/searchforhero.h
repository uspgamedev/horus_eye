#ifndef HORUSEYE_GAME_AI_BLOCK_SEARCHFORHERO_H_
#define HORUSEYE_GAME_AI_BLOCK_SEARCHFORHERO_H_

#include "game/ai/ailogicblock.h"

namespace ai {

class SearchForHero : public AILogicBlock {

public:
	SearchForHero(LogicModule* parent) : AILogicBlock(parent) {}
	~SearchForHero() {}

	virtual void Start();
	virtual AIModule::Status Update(float dt);
	virtual void Finish();
};

}
#endif // SEARCHFORHERO_H_