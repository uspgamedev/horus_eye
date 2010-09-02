#include "../utils/visionstrategy.h"
#include "../utils/imagefactory.h"
#include "pharaoh.h"
#include "weapons/weapon.h"

using namespace std;
using namespace framework;
using namespace scene;
using namespace utils;

namespace sprite {

#define SQRT_3 1.7320508075688772935274463415059f

Pharaoh::Pharaoh(int life) : Mummy() {
	ImageFactory img_fac;
    Initialize(img_fac.PharaohImage()); 

	life_ = max_life_ = life;
	mana_ = max_mana_ = 0;

    time_to_think_ = PHARAOH_TIME_TO_THINK;
    standing_ = true;
}

Pharaoh::~Pharaoh() {
	delete ranged_weapon_;
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
		}
        else if(!standing_){
            RandomMovement();
            last_standing_animation_ = *(standing_animations_[animation_direction_]);
        }
    }
}

}
