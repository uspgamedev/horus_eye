#include "../utils/visionstrategy.h"
#include "../utils/imagefactory.h"
#include "../utils/constants.h"
#include "pharaoh.h"
#include "weapons/weapon.h"

using namespace std;
using namespace framework;
using namespace scene;
using namespace utils;

namespace sprite {

#define SQRT_3 1.7320508075688772935274463415059

//Mana Regen Rate is how many secs to restore 1 mana point.
#define MANA_REGEN_RATE Constants::PHARAOH_MANA_REGEN_RATE
#define SUMMON_MANA_COST Constants::PHARAOH_SUMMON_MANA_COST
#define SUMMON_RANGED_CHANCE  30
#define SUMMON_BIG_CHANCE     20


Pharaoh::Pharaoh(int life, int mana) : Mummy() {
	ImageFactory img_fac;
    Initialize(img_fac.PharaohImage()); 

	life_ = max_life_ = life;
	mana_ = max_mana_ = mana;
	mana_regen_time_ = MANA_REGEN_RATE;

    time_to_think_ = PHARAOH_TIME_TO_THINK;
    standing_ = true;
	hit_duration_ = new TimeAccumulator(0);
}

Pharaoh::~Pharaoh() {
	delete ranged_weapon_;
}

void Pharaoh::TakeDamage(int life_points) {
    Creature::TakeDamage(life_points);
    PlayHitSound();
    if(life_ > 0) {
        hit_duration_->Restart(1000);
        blink_time_ = 0;
    }
    standing_ = false;
}

void Pharaoh::Update(float delta_t) {
	if (status_ == WorldObject::STATUS_DEAD) return;
	Mummy::Update(delta_t);

	if (!hit_duration_->Expired()) {
        blink_time_ += delta_t;
        if (blink_time_ > 0.04) {
            blink_ = !blink_;
            blink_time_ = 0;
        }
    }
    else if (blink_) {
        blink_ = false;
    }

	mana_regen_time_ -= delta_t;
	if (mana_regen_time_ < 0) {
		mana_regen_time_ = MANA_REGEN_RATE;
		mana_++;
		if (mana_ > max_mana_) mana_ = max_mana_;
	}
}

void Pharaoh::StartSummonMummy(Creature* target) {
	
	mana_ -= SUMMON_MANA_COST;

	Vector2D mummyPos = (Vector2D::Normalized(target->world_position() - world_position()))*2.0f + world_position();
	scene::World *world = WORLD();
	/* The choice of mummy type to summon here is based on a discrete percent probability.
	   A value in the range [0, 100[ is chosen randomly, and then depending on where it
	   sits in the range, according to boundaries determined by our SUMMON_<>_CHANCE defines,
	   a given mummy type is chosen. Example:
                                 choice = 34
                                   V
	   [ ----RANGED_CHANCE--- || ----BIG_CHANCE---- || ------- Rest of bar is normal mummy ----- ] 
	   0                      30                    50                                           100
	   So in this example, we will summon a big mummy.
	*/
	int choice = rand()%100;
	if (choice < SUMMON_RANGED_CHANCE) {
		world->AddRangedMummy(mummyPos);
	}
	else if (choice < SUMMON_RANGED_CHANCE + SUMMON_BIG_CHANCE) {
		world->AddBigMummy(mummyPos);
	}
	else {
		world->AddMummy(mummyPos);
	}

	float attackAngle = GetAttackingAngle(target->position() - position());
    int attackAnimationIndex = GetAttackingAnimationIndex(attackAngle);
    waiting_animation_ = true;
    last_standing_animation_ = *standing_animations_[direction_mapping_[attackAnimationIndex]];
    this->SelectAnimation(attacking_animations_[attackAnimationIndex]);
}

void Pharaoh::Think(float dt) {
    time_to_think_ -= dt;
    if(time_to_think_ <= 0){
        time_to_think_ = PHARAOH_TIME_TO_THINK;
		speed_ = original_speed_;
        VisionStrategy strategy;
        if(strategy.IsVisible(world_position())){
            standing_ = false;
			
			path_ = strategy.Calculate(world_position());
			UpdateDirection(path_.front());
			
			Vector2D diff;
			diff = path_.front() - world_position();
			if(diff.length() <= weapon_->range()){
				weapon_->Attack();
				speed_ = 0;
			}
			else if ( (ranged_weapon_->range()/2.0f <= diff.length()) & (diff.length() <= ranged_weapon_->range()) ) {
				ranged_weapon_->Attack();
				speed_ = 0;
			}
			else if (mana_ >= SUMMON_MANA_COST) {
				scene::World *world = WORLD();
				Hero* hero = world->hero();
				StartSummonMummy(hero);
				speed_ = 0;
			}
		}
        else if(!standing_){
            RandomMovement();
            last_standing_animation_ = *(standing_animations_[animation_direction_]);
        }
    }
}

}
