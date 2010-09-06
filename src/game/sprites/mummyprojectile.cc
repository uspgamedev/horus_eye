//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/projectile.cc
// projectile.cc da classe Projectile
//

#include "mummyprojectile.h"
#include "hero.h"
#include "../../framework/engine.h"
#include "../../framework/timeaccumulator.h"
#include "../../framework/videomanager.h"
#include "../scenes/world.h"
#include "../utils/circleobject.h"
#include "../utils/constants.h"
#include "../utils/imagefactory.h"

using namespace framework;
using namespace scene;
using namespace utils;

#define CENTER_X    Constants::PROJECTILE_SPRITE_CENTER_X
#define CENTER_Y    Constants::PROJECTILE_SPRITE_CENTER_Y
#define HEIGHT      Constants::PROJECTILE_HEIGHT
#define PROJECTILE_SPEED    Constants::PROJECTILE_SPEED
#define PROJECTILE_DURATION Constants::PROJECTILE_DURATION
#define PROJECTILE_SPRITE_WIDTH    Constants::PROJECTILE_SPRITE_WIDTH
#define PROJECTILE_SPRITE_HEIGHT   Constants::PROJECTILE_SPRITE_HEIGHT

namespace sprite {

MummyProjectile::MummyProjectile(Vector2D & dir, int damage) :
        direction_(Vector2D::Normalized(dir))
{
    ImageFactory image_factory;
    Initialize( image_factory.ProjectileImage() );
	set_hotspot( Vector2D(CENTER_X, CENTER_Y + PROJECTILE_SPRITE_HEIGHT + HEIGHT) );
	damage_ = damage;
    speed_ = PROJECTILE_SPEED;
    this->bound_ = new CircleObject(0.15f);
    duration_ = new TimeAccumulator(PROJECTILE_DURATION);
    this->collision_type_ = MOVEABLE;
}

MummyProjectile::~MummyProjectile() {
    delete duration_;
}

void MummyProjectile::Move(float delta_t) {
    Vector2D velocity = direction_ * (speed_ * delta_t);
    set_world_position(this->world_position() + velocity);
}

void MummyProjectile::Update(float delta_t) {
	if( duration_->Expired() ) {
	    this->status_ = WorldObject::STATUS_DEAD;
	}
	WorldObject::Update(delta_t);
	this->Move(delta_t);
}

void MummyProjectile::CollidesWith(Wall * obj) {
    this->status_ = WorldObject::STATUS_DEAD;
}

void MummyProjectile::CollidesWith(Door * obj) {
    this->status_ = WorldObject::STATUS_DEAD;
}

void MummyProjectile::CollidesWith(Hero *obj) {
    this->status_ = WorldObject::STATUS_DEAD;
}

void MummyProjectile::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

}
