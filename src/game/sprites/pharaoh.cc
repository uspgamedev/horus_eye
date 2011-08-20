#include "../utils/visionstrategy.h"
#include "../utils/imagefactory.h"
#include "../utils/constants.h"
#include "pharaoh.h"
#include "weapons/weapon.h"
#include "mummybuilder.h"
#include "../../framework/vector2D.h"
using namespace std;
using namespace ugdk;
using namespace scene;
using namespace utils;

namespace sprite {

#define SQRT_3 1.7320508075688772935274463415059
#define PHARAOH_TIME_TO_THINK 0.05f

Pharaoh::Pharaoh(Image* image, int life, int mana) : Mummy(image) {
	life_ = max_life_ = life;
	mana_ = max_mana_ = mana;
	mana_regen_ = Constants::PHARAOH_MANA_REGEN;

    time_to_think_ = PHARAOH_TIME_TO_THINK;
    standing_ = true;
}

Pharaoh::~Pharaoh() {
	delete ranged_weapon_;
	delete summon_weapon_;
}

void Pharaoh::TakeDamage(int life_points) {
	if(hit_duration_->Expired()){
		Creature::TakeDamage(life_points);
		PlayHitSound();
		if(life_ > 0) {
			hit_duration_->Restart(1000);
			blink_time_ = 0;
		}
		standing_ = false;
	}
}

void Pharaoh::Update(float delta_t) {
	if (status_ == WorldObject::STATUS_DEAD) return;
	Mummy::Update(delta_t);

	AdjustBlink(delta_t);
    set_mana(mana() + mana_regen_ * delta_t);
}

bool Pharaoh::CanAttackWithMeele(Vector2D diff) {
	if(diff.length() > weapon_->range()) return false;
	return weapon_->Available();
}

bool Pharaoh::CanAttackWithRangedWeapon(Vector2D diff) {
	if(diff.length() < ranged_weapon_->range()/2.0f) return false;
	if(diff.length() > ranged_weapon_->range()) return false;
	return ranged_weapon_->Available();
}

void Pharaoh::Think(float dt) {
	time_to_think_ -= dt;
	if(time_to_think_ <= 0) {
		time_to_think_ = PHARAOH_TIME_TO_THINK;
		speed_ = original_speed_;
		VisionStrategy strategy;
		if(strategy.IsVisible(world_position())) {
			standing_ = false;

			path_ = strategy.Calculate(world_position());
			UpdateDirection(path_.front());

			Vector2D diff = path_.front() - world_position();
			if(CanAttackWithMeele(diff)){
				weapon_->Attack();
				speed_ = 0;
			} else if(CanAttackWithRangedWeapon(diff)) {
				ranged_weapon_->Attack();
				speed_ = 0;
			} else if (summon_weapon_->Available()) {
			    summon_weapon_->Attack();
				speed_ = 0;
			}
		} else if(!standing_) {
			RandomMovement();
			last_standing_animation_ = standing_animations_[animation_direction_];
        }
    }
}

}
