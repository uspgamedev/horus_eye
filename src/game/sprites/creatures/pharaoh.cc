
#include <ugdk/math/vector2D.h>

#include "pharaoh.h"

#include "game/utils/visionstrategy.h"
#include "game/utils/imagefactory.h"
#include "game/utils/constants.h"
#include "game/skills/combatart.h"

using namespace std;
using namespace ugdk;
using namespace utils;
using resource::Energy;

namespace sprite {

#define SQRT_3 1.7320508075688772935274463415059
#define PHARAOH_TIME_TO_THINK 0.05

Pharaoh::Pharaoh(FlexibleSpritesheet* image, int life, int mana) : Mummy(image) {
	life_ = Energy(life);
	mana_ = Energy(mana, Constants::PHARAOH_MANA_REGEN);
    identifier_ = "Pharaoh";

    time_to_think_ = PHARAOH_TIME_TO_THINK;
    standing_ = true;
    invulnerability_time_ = 1000;
    super_armor_ = true;
}

Pharaoh::~Pharaoh() {
	delete ranged_weapon_;
	delete summon_weapon_;
}

void Pharaoh::Update(double delta_t) {
	Mummy::Update(delta_t);
    AdjustBlink(delta_t);
}

void Pharaoh::Think(double dt) {
    Creature::Update(dt);
	time_to_think_ -= dt;
	if(time_to_think_ <= 0) {
		time_to_think_ = PHARAOH_TIME_TO_THINK;
		speed_ = original_speed_;
		VisionStrategy strategy;
		if(strategy.IsVisible(world_position())) {
			standing_ = false;

			path_ = strategy.Calculate(world_position());
			UpdateDirection(path_.front());

            aim_destination_ = path_.front();
            if(weapon_->Available() && weapon_->IsValidUse()){
				weapon_->Use();
                this->StartAttack(NULL);
				speed_ = 0;

			} else if(ranged_weapon_->Available() && ranged_weapon_->IsValidUse()) {
				ranged_weapon_->Use();
                this->StartAttack(NULL);
				speed_ = 0;

			} else if (summon_weapon_->Available() && summon_weapon_->IsValidUse()) {
			    summon_weapon_->Use();
                this->StartAttack(NULL);
				speed_ = 0;
			}

		} else if(!standing_) {
			RandomMovement();
			last_standing_animation_ = standing_animations_[animation_direction_];
        }
    }
}

}
