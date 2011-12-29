#include "aibuilder.h"
#include "game/sprites/creatures/mummy.h"
#include "game/sprites/creatures/pharaoh.h"
#include "game/ai/ai.h"
#include "game/ai/sequencemodule.h"
#include "game/ai/logicmodule.h"
#include "game/ai/blocks/randommovement.h"
#include "game/ai/blocks/checkherovisible.h"
#include "game/ai/blocks/updatedirections.h"
#include "game/ai/blocks/useweapon.h"
#include "game/ai/blocks/move.h"

#define EXP_PARAM (1.0f)

namespace builder {

using namespace ai;
/*Always remember: we should create the AIs (and their AIModule trees) in a top-down method...
				like in the following order:
	AI -> root AIModule -> AI.SetRootModule -> [ other module -> set module as child from parent ]* -> return AI */

AI* AIBuilder::BasicMummyAI(sprite::Mummy* owner) {
	AI* ai_ = new AI( (sprite::Creature*)owner );

	SequenceModule* mainList = new SequenceModule();
	ai_->SetRootModule(mainList);

	LogicModule* checkForHero = new LogicModule();
	new CheckHeroVisible(checkForHero);
	mainList->AddChildModule(checkForHero);

	LogicModule* randomMove = new LogicModule();
	new RandomMovement(randomMove, EXP_PARAM);
	mainList->AddChildModule(randomMove);

	LogicModule* updateAimAndDirection = new LogicModule();
	new UpdateDirections(updateAimAndDirection);
	checkForHero->SetChildModule(updateAimAndDirection);

	SequenceModule* actionList = new SequenceModule();
	updateAimAndDirection->SetChildModule(actionList);

	LogicModule* doAttack = new LogicModule();
	new UseWeapon(doAttack, owner->get_weapon());
	actionList->AddChildModule(doAttack);

	LogicModule* doMove = new LogicModule();
	new Move(doMove);
	actionList->AddChildModule(doMove);

	return ai_;
}

AI* AIBuilder::BasicPharaohAI(sprite::Pharaoh* owner) {
	AI* ai_ = new AI( (sprite::Creature*)owner );

	SequenceModule* mainList = new SequenceModule();
	ai_->SetRootModule(mainList);

	LogicModule* checkForHero = new LogicModule();
	new CheckHeroVisible(checkForHero);
	mainList->AddChildModule(checkForHero);

	LogicModule* randomMove = new LogicModule();
	new RandomMovement(randomMove, EXP_PARAM);
	mainList->AddChildModule(randomMove);

	LogicModule* updateAimAndDirection = new LogicModule();
	new UpdateDirections(updateAimAndDirection);
	checkForHero->SetChildModule(updateAimAndDirection);

	SequenceModule* actionList = new SequenceModule();
	updateAimAndDirection->SetChildModule(actionList);

	LogicModule* doAttack = new LogicModule();
	new UseWeapon(doAttack, owner->get_weapon());
	actionList->AddChildModule(doAttack);

	LogicModule* doRangedAttack = new LogicModule();
	new UseWeapon(doRangedAttack, owner->get_ranged_weapon());
	actionList->AddChildModule(doRangedAttack);

	LogicModule* doSummonAttack = new LogicModule();
	new UseWeapon(doSummonAttack, owner->get_summon_weapon());
	actionList->AddChildModule(doSummonAttack);

	LogicModule* doMove = new LogicModule();
	new Move(doMove);
	actionList->AddChildModule(doMove);

	return ai_;
}

}
