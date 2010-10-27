#include "explosion.h"
#include "mummy.h"
#include "../../framework/animation.h"
#include "../../framework/timeaccumulator.h"
#include "../utils/circleobject.h"
#include "../utils/constants.h"

using namespace framework;
using namespace utils;

#define CENTER_X    Constants::EXPLOSION_SPRITE_CENTER_X
#define CENTER_Y    Constants::EXPLOSION_SPRITE_CENTER_Y
#define HEIGHT      Constants::EXPLOSION_HEIGHT
#define EXPLOSION_SPRITE_WIDTH    Constants::EXPLOSION_SPRITE_WIDTH
#define EXPLOSION_SPRITE_HEIGHT   Constants::EXPLOSION_SPRITE_HEIGHT

namespace sprite {

Explosion::Explosion(Image *image, Animation *animation, float radius, float damage) 
{
    Initialize(image);
	set_hotspot(Vector2D(CENTER_X, CENTER_Y));
	damage_ = damage;
	bound_ = new CircleObject(radius / 2);
    light_radius_ = 1.3*radius;
	collision_type_ = MOVEABLE;
    animation->AddObserver(this);
    SelectAnimation(animation);

    expansion_speed_ = (radius / 2) / (animation->n_frames() / animation->fps());
}

Explosion::~Explosion() {}

void Explosion::Tick() {
    this->status_ = WorldObject::STATUS_DEAD;
}

void Explosion::RadiusUpdate(float delta_t) {
	CircleObject* bound = static_cast<CircleObject*>(this->bound_);
	bound->set_radius(bound->radius() + expansion_speed_ * delta_t);
}

void Explosion::Update(float delta_t) {
    WorldObject::Update(delta_t);
	this->RadiusUpdate(delta_t);
}

void Explosion::CollidesWith(Mummy *obj) {
    obj->TakeDamage(damage_);
}

void Explosion::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

}
