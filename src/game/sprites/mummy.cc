
#include "../../framework/engine.h"
#include "../../framework/scene.h"
#include "../../framework/image.h"
#include "../../framework/sprite.h"
#include "../../framework/videomanager.h"
#include "../../framework/inputmanager.h"
#include "../../framework/timehandler.h"


#include "../scenes/world.h"
#include "../utils/geometryprimitives.h"
#include "../utils/visionstrategy.h"
#include "../utils/constants.h"
#include "mummy.h"
#include "projectile.h"
#include "weapon.h"
#include <cmath>
#include <iostream>
#include <sstream>

using namespace std;
using namespace framework;
using namespace scene;
using namespace utils;

namespace sprite {

#define SQRT_3 1.7320508075688772935274463415059
#define EXP_PARAM (1.0)

// Devolve um tempo ~exp(EXP_PARAM)
static int WaitingTime () {

    return (int)(1000*-log(1.0*rand()/RAND_MAX)/EXP_PARAM);

}

Mummy::Mummy(Image* img) {
    Initialize(img);
	bound_ = NULL;

    World *world = ((World *)Engine::reference()->CurrentScene());
    directions_[Direction_::RIGHT] = world->FromScreenLinearCoordinates(Vector2D(SQRT_3/2, 0));
    directions_[Direction_::LEFT] =  world->FromScreenLinearCoordinates(Vector2D(-SQRT_3/2, 0));
    directions_[Direction_::DOWN] =  world->FromScreenLinearCoordinates(Vector2D(0, .5));
    directions_[Direction_::UP] =  world->FromScreenLinearCoordinates(Vector2D(0, -.5));

    // Animations
    last_standing_animation_ = new Animation(0, -1);
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

    taking_damage_animation_ = new Animation(10, 80, 81, 82, -1);
    dying_animation_ = new Animation(10, 80, 81, 82, 83, 84, 90, 91, -1);

    animation_direction_ = 0;
    last_standing_animation_ = *standing_animations_[Animation_::DOWN];

    taking_damage_animation_->AddObserver(this);
    dying_animation_->AddObserver(this);

    this->SelectAnimation(last_standing_animation_);
    time_to_think_ = TIME_TO_THINK;
    standing_ = true;
    interval_ = new TimeAccumulator(0);
}

Mummy::~Mummy() {
    if (interval_) delete interval_;
	delete weapon_;
	((World*)(Engine::reference()->CurrentScene()))->DecreaseEnemyCount();
}

void Mummy::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

void Mummy::CollidesWith(Projectile* obj) {
    life_--;
    if (life_ == 0) {
        this->SelectAnimation(dying_animation_);
        this->status_ = WorldObject::STATUS_DYING;
        this->collision_type_ = WorldObject::NO_COLLISION;
        PlayHitSound();
    } else {
        waiting_animation_ = true;
        this->SelectAnimation(taking_damage_animation_);
    }
}

void Mummy::CollidesWith(Hero* obj) {
}

void Mummy::CollidesWith(Mummy *obj) {
    Vector2D deviation = Vector2D::Normalized(world_position() - obj->world_position());
    walking_direction_ = Vector2D::Normalized(walking_direction_ + deviation*0.9);
}

void Mummy::StartAttack(Creature* obj) {
    double attackAngle = GetAttackingAngle(obj->position() - position());
    int attackAnimationIndex = GetAttackingAnimationIndex(attackAngle);
    waiting_animation_ = true;
    last_standing_animation_ = *standing_animations_[direction_mapping_[attackAnimationIndex]];
    this->SelectAnimation(attacking_animations_[attackAnimationIndex]);
}

void Mummy::RandomMovement(){
    double PI = acos(-1);

    if (interval_->Expired()) {

        int dir = rand()%8;

        animation_direction_ = 0;
        if (dir < 3) animation_direction_ += Animation_::UP;
        if (dir >= 2 && dir < 5) animation_direction_ += Animation_::LEFT;
        if (dir >= 4 && dir < 7) animation_direction_ += Animation_::DOWN;
        if (dir >= 6 || dir == 0) animation_direction_ += Animation_::RIGHT;

        interval_->Restart(WaitingTime());
        last_direction_ = walking_direction_ = Vector2D(cos(dir*PI/4),sin(dir*PI/4));
    }
}

void Mummy::UpdateDirection(Vector2D destiny){
    Vector2D dir_animation = World::FromWorldCoordinates(destiny) - position(); 
    double angle = GetAttackingAngle(dir_animation);
    int dir = GetAttackingAnimationIndex(angle);

    animation_direction_ = direction_mapping_[dir];

    Vector2D dir_ = path_.front() - world_position(); 
    last_direction_ = walking_direction_ = Vector2D::Normalized(dir_);
    last_standing_animation_ = *(standing_animations_[animation_direction_]);

}

void Mummy::Think(float dt) {
    time_to_think_ -= dt;
    if(time_to_think_ <= 0){
        time_to_think_ = TIME_TO_THINK;
        VisionStrategy strategy;
        if(strategy.IsVisible(world_position())){
            standing_ = false;
            path_ = strategy.Calculate(world_position());
            UpdateDirection(path_.front());

			Vector2D diff;
			diff = path_.front() - world_position();
			if(diff.length() <= weapon_->range())
				weapon_->Attack();
        }
        else if(!standing_){
            RandomMovement();
            last_standing_animation_ = *(standing_animations_[animation_direction_]);
        }
    }
}

void Mummy::Update(float delta_t) {

    if (status_ == WorldObject::STATUS_DEAD) return;

    Creature::Update(delta_t);
    Vector2D dir(0,0);

    if (!waiting_animation_ && status_ == WorldObject::STATUS_ACTIVE) {
        Think(delta_t);

		if(!waiting_animation_) {
	        if (animation_direction_ & Animation_::UP)
	            dir = dir + directions_[Direction_::UP];
	        if (animation_direction_ & Animation_::DOWN)
	            dir = dir + directions_[Direction_::DOWN];
	        if (animation_direction_ & Animation_::LEFT)
	            dir = dir + directions_[Direction_::LEFT];
	        if (animation_direction_ & Animation_::RIGHT)
	            dir = dir + directions_[Direction_::RIGHT];

	        Creature::Move(this->GetWalkingDirection(), delta_t);
	        walking_direction_ = last_direction_;
	        this->SelectAnimation(*walking_animations_[animation_direction_]);
		}
    }

}

void Mummy::PlayHitSound() const {
    std::stringstream ss;
    int id = 1 + (rand() % 4);

    ss << "data/samples/hit" << id << ".wav";
    Engine::reference()->audio_manager()->LoadSample(ss.str().c_str())->Play();
}

}
