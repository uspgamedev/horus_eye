#include "explosion.h"
#include <ugdk/base/engine.h>
#include <ugdk/action/animation.h>
#include <ugdk/action/animationset.h>
#include <ugdk/util/animationparser.h>
#include <ugdk/time/timeaccumulator.h>

#include "game/sprites/mummy.h"
#include "game/utils/circleobject.h"
#include "game/utils/constants.h"

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

	known_collisions_[Mummy::Collision()] = new Collisions::Damage(this);
}

Explosion::~Explosion() {}

void Explosion::InitializeAnimations() {
    ANIMATIONS = Engine::reference()->animation_loader().Load("data/animations/explosion.gdd");
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

void Explosion::Collisions::Damage::Handle(WorldObject *obj) {
	Creature *creature = (Creature *) obj;
    creature->TakeDamage(owner_->damage());
}

}
