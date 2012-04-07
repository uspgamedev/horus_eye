#include <cmath>
#include <sstream>
#include <ugdk/base/engine.h>
#include <ugdk/action/animation.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/spritesheet/flexiblespritesheet.h>
#include <ugdk/time/timeaccumulator.h>

#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>

#include "mummy.h"

#include "game/scenes/world.h"
#include "game/utils/geometryprimitives.h"
#include "game/utils/visionstrategy.h"
#include "game/utils/constants.h"
#include "game/utils/settings.h"
#include "game/utils/imagefactory.h"
#include "game/utils/tile.h"
#include "game/sprites/item.h"
#include "game/builders/itembuilder.h"
#include "game/skills/combatart.h"

namespace sprite {

using namespace ugdk;
using scene::World;
using namespace utils;

#define EXP_PARAM (1.0)

// Devolve um tempo ~exp(EXP_PARAM)
static int WaitingTime () {
    return (int)(1000*-log(1.0*rand()/RAND_MAX)/EXP_PARAM);
}

COLLISION_DIRECT(Mummy*, MummyAntiStackCollision, voiddata) {
    WorldObject *obj = (WorldObject *) voiddata; 
    data_->MummyAntiStack(obj);
}

Mummy::Mummy(ugdk::graphic::FlexibleSpritesheet* img) {
    Initialize(img, ANIMATIONS);

    // Animations
    last_standing_animation_ = standing_animations_[Animation_::DOWN];

    sprite_->SelectAnimation(last_standing_animation_);
    time_to_think_ = TIME_TO_THINK;
    standing_ = true;
    interval_ = new ugdk::time::TimeAccumulator(0);
    invulnerability_time_ = 300;

    identifier_ = std::string("Mummy");

    INITIALIZE_COLLISION;
    SET_COLLISIONCLASS(Mummy);
    Creature::AddKnownCollisions();
    ADD_COLLISIONLOGIC(Mummy, new MummyAntiStackCollision(this));
}

Mummy::~Mummy() {
    if (interval_) delete interval_;
	WORLD()->DecreaseEnemyCount();
}

void Mummy::TakeDamage(double life_points) {
    Creature::TakeDamage(life_points);
    standing_ = false;
}

void Mummy::MummyAntiStack(WorldObject *obj) {
    Vector2D deviation = (world_position() - obj->world_position()).Normalize();
    walking_direction_ = (walking_direction_ + deviation*0.9).Normalize();
}

void Mummy::StartAttack(WorldObject* obj) {
    if(obj == NULL) obj = WORLD()->hero_world_object();
    double attackAngle = GetAttackingAngle(obj->node()->modifier()->offset() - node()->modifier()->offset());
    int attackAnimationIndex = GetAttackingAnimationIndex(attackAngle);
    waiting_animation_ = true;
    last_standing_animation_ = standing_animations_[direction_mapping_[attackAnimationIndex]];
    sprite_->SelectAnimation(attacking_animations_[attackAnimationIndex]);
}

void Mummy::set_bound(double radius) {
    SET_COLLISIONSHAPE(new pyramidworks::geometry::Circle(radius));
}

void Mummy::RandomMovement(){
    double PI = acos(-1.0);

    if (interval_->Expired()) {

        int dir = rand()%8;

        animation_direction_ = 0;
        if (dir < 3) animation_direction_ += Animation_::UP;
        if (dir >= 2 && dir < 5) animation_direction_ += Animation_::LEFT;
        if (dir >= 4 && dir < 7) animation_direction_ += Animation_::DOWN;
        if (dir >= 6 || dir == 0) animation_direction_ += Animation_::RIGHT;

        interval_->Restart(WaitingTime());
        last_direction_ = walking_direction_ = Vector2D(cos(dir*PI/4.0),sin(dir*PI/4.0));
    }
}

void Mummy::UpdateDirection(Vector2D destination){
    Vector2D dir_animation = World::FromWorldCoordinates(destination) - node()->modifier()->offset(); 
    double angle = GetAttackingAngle(dir_animation);
    int dir = GetAttackingAnimationIndex(angle);

    animation_direction_ = direction_mapping_[dir];

    Vector2D dir_ = path_.front() - world_position(); 
    last_direction_ = walking_direction_ = Vector2D::Normalized(dir_);
    last_standing_animation_ = (standing_animations_[animation_direction_]);

}

void Mummy::Think(double dt) {
    time_to_think_ -= dt;
    if(time_to_think_ <= 0){
        time_to_think_ = TIME_TO_THINK;
		speed_ = original_speed_;
        VisionStrategy strategy;
        if(strategy.IsVisible(world_position())){
            standing_ = false;
			
			path_ = strategy.Calculate(world_position());
			UpdateDirection(path_.front());
			
            if(weapon_->Available()) {
                aim_destination_ = path_.front();
                if(weapon_->IsValidUse()){
				    weapon_->Use();
                    this->StartAttack(NULL);
				    speed_ = 0;
			    }
            }
		}
        else if(!standing_){
            RandomMovement();
            last_standing_animation_ = (standing_animations_[animation_direction_]);
        }
    }
}

void Mummy::Update(double delta_t) {
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
                node_->modifier()->set_visible(true);
            else
                node_->modifier()->set_visible(false);
        }
    }

    if (!waiting_animation_ && is_active()) {
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
	        sprite_->SelectAnimation(walking_animations_[animation_direction_]);
		}
    }

}

void Mummy::StartToDie() {
    Creature::StartToDie();
	int potion = rand() % 100;
	if (potion <=20){
		builder::ItemBuilder builder;
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

    ss << "samples/hit" << id << ".wav";
    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample(ss.str().c_str())->Play();
}

}
