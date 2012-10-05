#ifndef HORUSEYE_GAME_AI_BLOCK_SEARCHFORHERO_H_
#define HORUSEYE_GAME_AI_BLOCK_SEARCHFORHERO_H_

#include "game/ai.h"
#include "game/ai/logicblock.h"

namespace ai {
namespace blocks {

class SearchForHero : public LogicBlock {

public:
	SearchForHero() {}
	~SearchForHero() {}

	virtual void Start();
	virtual AIModule::Status Update(double dt, AIData* data);
	virtual void Finish();
};

}
}
#endif // SEARCHFORHERO_H_
