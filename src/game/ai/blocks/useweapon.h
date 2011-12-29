#ifndef HORUSEYE_GAME_AI_BLOCK_USEWEAPON_H_
#define HORUSEYE_GAME_AI_BLOCK_USEWEAPON_H_

#include "game/ai/ailogicblock.h"

namespace skills {
class Skill;
}

namespace ai {

class UseWeapon : public AILogicBlock {

public:
	UseWeapon(LogicModule* parent, skills::Skill* weapon) : AILogicBlock(parent), weapon_(weapon) {}
	~UseWeapon() {}

	virtual void Start();
	virtual AIModule::Status Update(float dt);
	virtual void Finish();

protected:
	skills::Skill* weapon_;
};

}
#endif // USEWEAPON_H_