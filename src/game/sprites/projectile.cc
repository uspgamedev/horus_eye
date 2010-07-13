//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/projectile.cc
// projectile.cc da classe Projectile
//

#include "projectile.h"
#include "../../framework/engine.h"
#include "../scenes/world.h"
#include "../utils/circleobject.h"
#include "../utils/constants.h"

using namespace framework;
using namespace scene;
using namespace utils;

#define CENTER_X    Constants::PROJECTILE_SPRITE_CENTER_X
#define CENTER_Y    Constants::PROJECTILE_SPRITE_CENTER_Y
#define HEIGHT      Constants::PROJECTILE_HEIGHT

namespace sprite {

Projectile::Projectile(Vector2D & pos, Vector2D & dir) :
        direction_(Vector2D::Normalized(dir))
{
	// TODO: remover os numeros magicos
    Initialize(VIDEO_MANAGER()->LoadImage("data/images/fire_ball.png"));
    image()->set_frame_size(Vector2D(32,32));
    set_hotspot(Vector2D(CENTER_X, CENTER_Y + HEIGHT));
    dano_ = 1;
    speed_ = 8.0f;
    this->bound_ = new CircleObject(0.15f);
    this->set_world_position(pos);
    duration_ = new TimeAccumulator(5000);
    this->collision_type_ = MOVEABLE;
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
	this->Move(delta_t);
}

void Projectile::CollidesWith(Wall * obj) {
    this->status_ = WorldObject::STATUS_DEAD;
}

void Projectile::CollidesWith(Mummy *obj) {
    this->status_ = WorldObject::STATUS_DEAD;
}

void Projectile::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

}
