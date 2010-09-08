#include "explosion.h"
#include "mummy.h"
#include "../../framework/timeaccumulator.h"
#include "../utils/circleobject.h"
#include "../utils/constants.h"
#include "../utils/imagefactory.h"

using namespace framework;
using namespace utils;

#define CENTER_X    Constants::EXPLOSION_SPRITE_CENTER_X
#define CENTER_Y    Constants::EXPLOSION_SPRITE_CENTER_Y
#define HEIGHT      Constants::EXPLOSION_HEIGHT
#define EXPLOSION_SPRITE_WIDTH    Constants::EXPLOSION_SPRITE_WIDTH
#define EXPLOSION_SPRITE_HEIGHT   Constants::EXPLOSION_SPRITE_HEIGHT

namespace sprite {

Explosion::Explosion() 
{
    ImageFactory image_factory;
    Initialize( image_factory.ExplosionImage());
	set_hotspot(Vector2D(CENTER_X, CENTER_Y));
	damage_ = Constants::EXPLOSION_DAMAGE;
	bound_ = new CircleObject(0.15f);
    light_radius_ = 4.0f;
	collision_type_ = MOVEABLE;
    Animation *animation = new Animation(10, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, -1);
    animation->AddObserver(this);
    SelectAnimation(animation);
}

Explosion::~Explosion() {}

void Explosion::Tick() {
    this->status_ = WorldObject::STATUS_DEAD;
}

void Explosion::RadiusUpdate(float delta_t) {
	CircleObject* bound = static_cast<CircleObject*>(this->bound_);
	bound->set_radius(Constants::EXPLOSION_DAMAGE_RADIUS);
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
