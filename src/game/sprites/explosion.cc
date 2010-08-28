//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/projectile.cc
// projectile.cc da classe Explosion
//

#include "explosion.h"
#include "mummy.h"
#include "../../framework/timeaccumulator.h"
#include "../utils/circleobject.h"
#include "../utils/constants.h"
#include "../utils/imagefactory.h"

using namespace framework;
using namespace utils;

#define CENTER_X    Constants::PROJECTILE_SPRITE_CENTER_X
#define CENTER_Y    Constants::PROJECTILE_SPRITE_CENTER_Y
#define HEIGHT      Constants::PROJECTILE_HEIGHT
#define PROJECTILE_SPRITE_WIDTH    Constants::PROJECTILE_SPRITE_WIDTH
#define PROJECTILE_SPRITE_HEIGHT   Constants::PROJECTILE_SPRITE_HEIGHT

namespace sprite {

Explosion::Explosion(Vector2D & pos, Vector2D & dir) :
			direction_(Vector2D::Normalized(dir))
{
    ImageFactory image_factory;
    Initialize( image_factory.ProjectileImage() );
	set_hotspot( Vector2D(CENTER_X, CENTER_Y + PROJECTILE_SPRITE_HEIGHT + HEIGHT) );
	damage_ = 1;
	speed_ = Constants::PROJECTILE_SPEED;
	bound_ = new CircleObject(0.15f);
	set_world_position(pos);
	light_radius_ = 1.5f;
	duration_ = new TimeAccumulator(Constants::PROJECTILE_DURATION);
	collision_type_ = MOVEABLE;
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
        this->status_ = WorldObject::STATUS_DEAD;
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
    if (!already_hit_.count(obj)) {
        obj->TakeDamage();
        already_hit_.insert(obj);
    }
	Explode();
}
void Explosion::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}
void Explosion::Explode(){
	if(!exploding_) {
		exploding_ = true;
		duration_->Restart(Constants::EXPLOSION_DURATION);
	}
}

}
