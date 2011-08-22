#include <cmath>
#include <iostream>
#include <sstream>
#include "ugdk/engine.h"
#include "ugdk/scene.h"
#include "ugdk/image.h"
#include "ugdk/sprite.h"
#include "ugdk/animation.h"
#include "ugdk/videomanager.h"
#include "ugdk/inputmanager.h"
#include "ugdk/timehandler.h"
#include "ugdk/audiomanager.h"
#include "ugdk/timeaccumulator.h"

#include "../scenes/world.h"
#include "../utils/geometryprimitives.h"
#include "../utils/visionstrategy.h"
#include "../utils/constants.h"
#include "../utils/settings.h"
#include "../utils/imagefactory.h"
#include "../utils/tile.h"
#include "mummy.h"
#include "projectile.h"
#include "explosion.h"
#include "itembuilder.h"
#include "weapons/weapon.h"

using namespace std;
using namespace ugdk;
using namespace scene;
using namespace utils;

namespace sprite {

#define SQRT_3 1.7320508075688772935274463415059f
#define EXP_PARAM (1.0f)

// Devolve um tempo ~exp(EXP_PARAM)
static int WaitingTime () {

    return (int)(1000*-log(1.0*rand()/RAND_MAX)/EXP_PARAM);

}

Mummy::Mummy(Image* img) {
    Initialize(img, ANIMATIONS);
	bound_ = NULL;

    // Animations  
    animation_direction_ = 0;
    last_standing_animation_ = standing_animations_[Animation_::DOWN];

    this->SelectAnimation(last_standing_animation_);
    time_to_think_ = TIME_TO_THINK;
    standing_ = true;
    interval_ = new TimeAccumulator(0);
    invulnerability_time_ = 300;
}

Mummy::~Mummy() {
    if (interval_) delete interval_;
	delete weapon_;
	WORLD()->DecreaseEnemyCount();
}

void Mummy::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

void Mummy::TakeDamage(float life_points) {
    Creature::TakeDamage(life_points);
    standing_ = false;
}

void Mummy::CollidesWith(Mummy *obj) {
    Vector2D deviation = Vector2D::Normalized(world_position() - obj->world_position());
    walking_direction_ = Vector2D::Normalized(walking_direction_ + deviation*0.9f);
}

void Mummy::StartAttack(Creature* obj) {
    float attackAngle = GetAttackingAngle(obj->position() - position());
    int attackAnimationIndex = GetAttackingAnimationIndex(attackAngle);
    waiting_animation_ = true;
    last_standing_animation_ = standing_animations_[direction_mapping_[attackAnimationIndex]];
    this->SelectAnimation(attacking_animations_[attackAnimationIndex]);
}

void Mummy::RandomMovement(){
    float PI = acos(-1.0f);

    if (interval_->Expired()) {

        int dir = rand()%8;

        animation_direction_ = 0;
        if (dir < 3) animation_direction_ += Animation_::UP;
        if (dir >= 2 && dir < 5) animation_direction_ += Animation_::LEFT;
        if (dir >= 4 && dir < 7) animation_direction_ += Animation_::DOWN;
        if (dir >= 6 || dir == 0) animation_direction_ += Animation_::RIGHT;

        interval_->Restart(WaitingTime());
        last_direction_ = walking_direction_ = Vector2D(cos(dir*PI/4.0f),sin(dir*PI/4.0f));
    }
}

void Mummy::UpdateDirection(Vector2D destiny){
    Vector2D dir_animation = World::FromWorldCoordinates(destiny) - position(); 
    float angle = GetAttackingAngle(dir_animation);
    int dir = GetAttackingAnimationIndex(angle);

    animation_direction_ = direction_mapping_[dir];

    Vector2D dir_ = path_.front() - world_position(); 
    last_direction_ = walking_direction_ = Vector2D::Normalized(dir_);
    last_standing_animation_ = (standing_animations_[animation_direction_]);

}

void Mummy::Think(float dt) {
    time_to_think_ -= dt;
    if(time_to_think_ <= 0){
        time_to_think_ = TIME_TO_THINK;
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
		}
        else if(!standing_){
            RandomMovement();
            last_standing_animation_ = (standing_animations_[animation_direction_]);
        }
    }
}

void Mummy::Update(float delta_t) {

    if (status_ == WorldObject::STATUS_DEAD) return;
    Creature::Update(delta_t);
    Vector2D dir(0,0);

    World *world = WORLD();
    if (world) {
        GameMap& map = world->level_matrix();
        TilePos mummy_pos = Tile::ToTilePos(world_position());
        mummy_pos.i =  map.size() - mummy_pos.i - 1;
        Tile *mummy_tile = Tile::GetFromMapPosition(map, mummy_pos);
        if (mummy_tile) {
            if(mummy_tile->visible())
                set_visible(true);
            else
                set_visible(false);
        }
    }

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
	        this->SelectAnimation(walking_animations_[animation_direction_]);
		}
    }

}

void Mummy::Die(){
	int potion = rand()%100;
		if (potion <=20){
			ItemBuilder builder;
			ImageFactory* image_factory = WORLD()->image_factory();
			if(potion > 10)
				WORLD()->AddWorldObject(builder.LifePotion(image_factory->LifePotionImage()), this->last_stable_position_);
			else if(potion> 5)
				WORLD()->AddWorldObject(builder.ManaPotion(image_factory->ManaPotionImage()), this->last_stable_position_);
			else
				WORLD()->AddWorldObject(builder.SightPotion(image_factory->SightPotionImage()), this->last_stable_position_);
		}
}

void Mummy::PlayHitSound() const {
    std::stringstream ss;
    int id = 1 + (rand() % 4);

    ss << "data/samples/hit" << id << ".wav";
    Settings settings;
    if(settings.sound_effects())
        Engine::reference()->audio_manager()->LoadSample(ss.str().c_str())->Play();
}

}
