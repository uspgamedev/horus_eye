//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/projectile.cc
// projectile.cc da classe Explosion
//

#include "explosion.h"
#include "../../framework/engine.h"
#include "../scenes/world.h"
#include "../utils/circleobject.h"
#include "../utils/constants.h"

using namespace framework;
using namespace scene;
using namespace utils;

//#define EXPLOSION_RADIUS
//#define EXPLOSION_DURATION

#define CENTER_X    Constants::PROJECTILE_SPRITE_CENTER_X
#define CENTER_Y    Constants::PROJECTILE_SPRITE_CENTER_Y
#define HEIGHT      Constants::PROJECTILE_HEIGHT
#define PROJECTILE_SPEED    Constants::PROJECTILE_SPEED
#define PROJECTILE_DURATION Constants::PROJECTILE_DURATION
#define PROJECTILE_SPRITE_WIDTH    Constants::PROJECTILE_SPRITE_WIDTH
#define PROJECTILE_SPRITE_HEIGHT   Constants::PROJECTILE_SPRITE_HEIGHT
#define EXPLOSION_DURATION 300

namespace sprite {

Explosion::Explosion(Vector2D & pos, Vector2D & dir) :
			direction_(Vector2D::Normalized(dir))
{
	Initialize( VIDEO_MANAGER()->LoadImage("data/images/fire_ball.png") );
	image()->set_frame_size( Vector2D(PROJECTILE_SPRITE_WIDTH, PROJECTILE_SPRITE_HEIGHT) );
	set_hotspot( Vector2D(CENTER_X, CENTER_Y + PROJECTILE_SPRITE_HEIGHT + HEIGHT) );
	damage_ = 1;
	speed_ = PROJECTILE_SPEED;
	this->bound_ = new CircleObject(0.15f);
	this->set_world_position(pos);
	duration_ = new TimeAccumulator(PROJECTILE_DURATION);
	this->collision_type_ = MOVEABLE;
	light_radius_ = 1.5f;
	exploding_ = false;
}

Explosion::~Explosion() {
    delete duration_;
}

void Explosion::Move(float delta_t) {
	Vector2D velocity = direction_ * (speed_ * delta_t);
	set_world_position(this->world_position() + velocity);
}

void Explosion::RadiusUpdate(float delta_t) {
	CircleObject* bound = static_cast<CircleObject*>(this->bound_);
	bound->set_radius(1.5f);
}

void Explosion::Update(float delta_t) {
	if( duration_->Expired() && exploding_) {
		/*if (this->exploding_ == false)
		{
			exploding_ = true;
			duration_->Restart(300);
		}
		else*/
		//{
			this->status_ = WorldObject::STATUS_DEAD;
		//}
	}
	WorldObject::Update(delta_t);
	if(!exploding_)
		this->Move(delta_t);
	else{
		this->RadiusUpdate(delta_t);
	}
}

void Explosion::CollidesWith(Wall * obj) {
	Explode();
}

void Explosion::CollidesWith(Door * obj) {
	Explode();
}
void Explosion::CollidesWith(Mummy *obj) {
	Explode();
}
void Explosion::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}
void Explosion::Explode(){
	if(!exploding_) {
		exploding_ = true;
		duration_->Restart(EXPLOSION_DURATION);
	}
}

}
