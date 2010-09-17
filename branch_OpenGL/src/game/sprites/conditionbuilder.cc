//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/conditionbuilder.cc
// conditionbuilder.cc da classe Condition
//
 
#include "conditionbuilder.h"
#include "../utils/constants.h"
#include "hero.h"
#include "creature.h"

namespace sprite {

using namespace utils;

void ConditionBuilder::IncreaseSightCondition::Update(float dt) {
	if ( phase_ != PHASE_FINISHED ) {
		if (phase_ == PHASE_IDLE) {
			StartCondition(owner_);
			phase_ = PHASE_ACTIVE;
		}
		condition_duration_ -= dt;
		if (condition_duration_ <= 0.0) 
		{
			EndCondition(owner_);
			phase_ = PHASE_FINISHED;
		}
	}
}

void ConditionBuilder::IncreaseSightCondition::StartCondition(Creature* obj) {
	obj->set_light_radius(obj->light_radius() + Constants::SIGHT_POTION_INCREASE);
	obj->set_sight_count(1);
}

void ConditionBuilder::IncreaseSightCondition::EndCondition(Creature* obj) {
	obj->set_light_radius(obj->light_radius() - Constants::SIGHT_POTION_INCREASE);
	obj->set_sight_count(-1);
}

Condition* ConditionBuilder::increase_sight_condition(Creature *owner) {
    return new IncreaseSightCondition(owner, Constants::CONDITION_DURATION);
}

}
