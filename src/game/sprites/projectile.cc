//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/projectile.cc
// projectile.cc da classe Projectile
//

#include "projectile.h"

using namespace framework;

namespace sprite {

Projectile::Projectile(Vector2D & pos, Vector2D & dir) :
        direction_(Vector2D::Normalized(dir))
{
	// TODO: remover os numeros magicos
    Initialize(VIDEO_MANAGER()->LoadImage("data/images/fire_ball.png"));
    image()->set_frame_size(Vector2D(32,32));
    set_hotspot(Vector2D(16, 16));
    dano_ = 1;
    speed_ = 5;
    this->set_world_position(pos);
    duration_ = new TimeAccumulator(5000);
}

void Projectile::Move(float delta_t) {
    Vector2D velocity = direction_ * (speed_ * delta_t);
    set_world_position(this->world_position() + velocity);
}

void Projectile::Update(float delta_t) {
	if( duration_->Expired() ) {
		// TODO: destruir isso
		return;
	}
	WorldObject::Update(delta_t);
	this->Move(delta_t);
}
/*
void CollidesWith(Wall * obj) {
	// TODO: destruir isso
}

void CollidesWith(Mummy *obj) {
}
*/

}
