//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/projectile.cc
// projectile.cc da classe Projectile
//

#include "projectile.h"
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

Projectile::Projectile(Vector2D & pos, Vector2D & dir) :
        direction_(Vector2D::Normalized(dir))
{
    ImageFactory image_factory;
    Initialize( image_factory.ProjectileImage() );
    set_hotspot( Vector2D(CENTER_X, CENTER_Y + PROJECTILE_SPRITE_HEIGHT + HEIGHT) );
	damage_ = Constants::PROJECTILE_DAMAGE;
    speed_ = Constants::PROJECTILE_SPEED;
    bound_ = new CircleObject(0.15f);
    set_world_position(pos);
    light_radius_ = 1.0f;
    duration_ = new TimeAccumulator(Constants::PROJECTILE_DURATION);
    collision_type_ = MOVEABLE;
}

Projectile::~Projectile() {
    delete duration_;
}

void Projectile::Move(float delta_t) {
    Vector2D velocity = direction_ * (speed_ * delta_t);
    set_world_position(this->world_position() + velocity);
}

void Projectile::Update(float delta_t) {
	if( duration_->Expired() ) {
	    this->status_ = WorldObject::STATUS_DEAD;
	}
	WorldObject::Update(delta_t);
	if (this->status_ == WorldObject::STATUS_ACTIVE)
	    this->Move(delta_t);
}

void Projectile::Explode() {
    if (this->status_ == WorldObject::STATUS_ACTIVE) {
        this->status_ = WorldObject::STATUS_DYING;
        duration_->Restart(250);
        set_visible(false);
    }
}

void Projectile::CollidesWith(Wall * obj) { Explode(); }
void Projectile::CollidesWith(Door * obj) { Explode(); }

void Projectile::CollidesWith(Mummy *obj) {
    if (this->status_ == WorldObject::STATUS_ACTIVE) {
        obj->TakeDamage();
    }
    Explode();
}

void Projectile::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

}
