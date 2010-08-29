#include <cmath>
#include <iostream>
#include <sstream>
#include "../../framework/engine.h"
#include "../../framework/scene.h"
#include "../../framework/image.h"
#include "../../framework/sprite.h"
#include "../../framework/animation.h"
#include "../../framework/videomanager.h"
#include "../../framework/inputmanager.h"
#include "../../framework/timehandler.h"
#include "../../framework/audiomanager.h"
#include "../../framework/timeaccumulator.h"

#include "../scenes/world.h"
#include "../utils/geometryprimitives.h"
#include "../utils/visionstrategy.h"
#include "../utils/constants.h"
#include "../utils/imagefactory.h"
#include "pharaoh.h"
#include "projectile.h"
#include "explosion.h"
#include "weapons/weapon.h"

using namespace std;
using namespace framework;
using namespace scene;
using namespace utils;

namespace sprite {

#define SQRT_3 1.7320508075688772935274463415059f
#define EXP_PARAM (1.0)

// Devolve um tempo ~exp(EXP_PARAM)
static int WaitingTime () {
    return (int)(1000*-log(1.0*rand()/RAND_MAX)/EXP_PARAM);
}

Pharaoh::Pharaoh(int life) {
	ImageFactory img_fac;
    Initialize(img_fac.PharaohImage());
	bound_ = NULL;
	life_ = max_life_ = life;
	mana_ = max_mana_ = 0;

    World *world = ((World *)Engine::reference()->CurrentScene());
    directions_[Direction_::RIGHT] = world->FromScreenLinearCoordinates(Vector2D(SQRT_3/2.0f, 0.0f));
    directions_[Direction_::LEFT] =  world->FromScreenLinearCoordinates(Vector2D(-SQRT_3/2.0f, 0.0f));
    directions_[Direction_::DOWN] =  world->FromScreenLinearCoordinates(Vector2D(0.0f, 0.5f));
    directions_[Direction_::UP] =  world->FromScreenLinearCoordinates(Vector2D(0.0f, -0.5f));

    // Animations
    InitializeStandingAnimations();
    InitializeWalkingAnimations();
    InitializeAttackingAnimations();
    
    direction_mapping_[0] = Animation_::RIGHT;
    direction_mapping_[1] = Animation_::RIGHT | Animation_::UP;
    direction_mapping_[2] = Animation_::UP;
    direction_mapping_[3] = Animation_::UP | Animation_::LEFT;
    direction_mapping_[4] = Animation_::LEFT;
    direction_mapping_[5] = Animation_::LEFT | Animation_::DOWN;
    direction_mapping_[6] = Animation_::DOWN;
    direction_mapping_[7] = Animation_::DOWN | Animation_::RIGHT;

	for(int i = 0;i < 8;i++) 
		attacking_animations_[i]->set_fps(5);

    taking_damage_animation_ = new Animation(10, 80, 81, 82, -1);
    dying_animation_ = new Animation(10, 80, 81, 82, 83, 84, 90, 91, -1);

    animation_direction_ = 0;
    last_standing_animation_ = *standing_animations_[Animation_::DOWN];

    taking_damage_animation_->AddObserver(this);
    dying_animation_->AddObserver(this);

    this->SelectAnimation(last_standing_animation_);
    time_to_think_ = PHARAOH_TIME_TO_THINK;
    standing_ = false;
    interval_ = new TimeAccumulator(0);
}

Pharaoh::~Pharaoh() {
	delete ranged_weapon_;
}

void Pharaoh::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

void Pharaoh::CollidesWith(Explosion* obj) {
    /*
    life_ -= obj->damage();
    if (life_ <= 0) {
        this->SelectAnimation(dying_animation_);
        this->status_ = WorldObject::STATUS_DYING;
        this->collision_type_ = WorldObject::NO_COLLISION;
        PlayHitSound();
    } else {
        waiting_animation_ = true;
        this->SelectAnimation(taking_damage_animation_);
    }*/
}

void Pharaoh::CollidesWith(Projectile* obj) {
    /*
    life_ -= obj->damage();
    if (life_ <= 0) {
        this->SelectAnimation(dying_animation_);
        this->status_ = WorldObject::STATUS_DYING;
        this->collision_type_ = WorldObject::NO_COLLISION;
    } else {
        waiting_animation_ = true;
        this->SelectAnimation(taking_damage_animation_);
    }*/
}

void Pharaoh::CollidesWith(Hero* obj) {
}

void Pharaoh::CollidesWith(Mummy *obj) {
    Vector2D deviation = Vector2D::Normalized(world_position() - obj->world_position());
    walking_direction_ = Vector2D::Normalized(walking_direction_ + deviation*0.9f);
}

void Pharaoh::StartAttack(Creature* obj) {
    double attackAngle = GetAttackingAngle(obj->position() - position());
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
		}
        else if(!standing_){
            RandomMovement();
            last_standing_animation_ = *(standing_animations_[animation_direction_]);
        }
    }
}

}
