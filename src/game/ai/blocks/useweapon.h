#ifndef HORUSEYE_GAME_AI_BLOCK_USEWEAPON_H_
#define HORUSEYE_GAME_AI_BLOCK_USEWEAPON_H_

#include "game/ai.h"
#include "game/ai/logicblock.h"
#include "game/components/controller.h"


namespace ai {
namespace blocks {

class UseWeapon : public LogicBlock {

public:
	UseWeapon(component::Controller::SkillSlot slot) : slot_(slot) {}
	~UseWeapon() {}

	virtual void Start();
	virtual AIModule::Status Update(double dt, AIData* data);
	virtual void Finish();

protected:
	component::Controller::SkillSlot slot_;
};

}
}
#endif // USEWEAPON_H_
