#include "explosion.h"
#include "mummy.h"
#include "../../framework/engine.h"
#include "../../framework/animation.h"
#include "../../framework/animationset.h"
#include "../../framework/animationparser.h"
#include "../../framework/timeaccumulator.h"
#include "../utils/circleobject.h"
#include "../utils/constants.h"

using namespace ugdk;
using namespace utils;

#define CENTER_X    Constants::EXPLOSION_SPRITE_CENTER_X
#define CENTER_Y    Constants::EXPLOSION_SPRITE_CENTER_Y
#define HEIGHT      Constants::EXPLOSION_HEIGHT
#define EXPLOSION_SPRITE_WIDTH    Constants::EXPLOSION_SPRITE_WIDTH
#define EXPLOSION_SPRITE_HEIGHT   Constants::EXPLOSION_SPRITE_HEIGHT

namespace sprite {

AnimationSet*   Explosion::ANIMATIONS = NULL;
const int       Explosion::HERO_FIREBALL_WEAPON = 0;
const int       Explosion::HERO_EXPLOSION_WEAPON = 1;
uint32          Explosion::WEAPON_ANIMATIONS[2];


Explosion::Explosion(Image *image, uint32 animation, float radius, float damage)
{
    Initialize(image, ANIMATIONS);
	set_hotspot(Vector2D(CENTER_X, CENTER_Y));
	damage_ = damage;
	bound_ = new CircleObject(radius / 2);
	set_light_radius(1.3*radius);
	collision_type_ = MOVEABLE;
	AddObserverToAnimation(this);
    SelectAnimation(WEAPON_ANIMATIONS[animation]);

    expansion_speed_ = (radius / 2) /
            (GetAnimationFrameNumber() / GetAnimationFPS());
}

Explosion::~Explosion() {}

void Explosion::InitializeAnimations() {
    ANIMATIONS = Engine::reference()->animation_parser()->Load("data/animations/explosion.and");
    WEAPON_ANIMATIONS[HERO_FIREBALL_WEAPON] =
            ANIMATIONS->MakeIndex("HERO_FIREBALL_WEAPON");
    WEAPON_ANIMATIONS[HERO_EXPLOSION_WEAPON] =
            ANIMATIONS->MakeIndex("HERO_EXPLOSION_WEAPON");
}

void Explosion::ReleaseAnimations() {
    ANIMATIONS = NULL;
    WEAPON_ANIMATIONS[HERO_FIREBALL_WEAPON] = -1;
    WEAPON_ANIMATIONS[HERO_EXPLOSION_WEAPON] = -1;
}

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
