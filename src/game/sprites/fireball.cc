#include <cmath>
#include "fireball.h"
#include "../scenes/world.h"
#include "mummy.h"
#include <ugdk/time/timeaccumulator.h>
#include "../utils/circleobject.h"
#include "../utils/constants.h"
#include "../utils/imagefactory.h"
#include "../scenes/world.h"
#include <ugdk/action/animation.h>
#include <ugdk/action/animationset.h>
#include <ugdk/util/animationparser.h>
#include <ugdk/base/engine.h>

using namespace ugdk;
using namespace utils;

#define CENTER_X    Constants::FIREBALL_SPRITE_CENTER_X
#define CENTER_Y    Constants::FIREBALL_SPRITE_CENTER_Y
#define HEIGHT      Constants::FIREBALL_HEIGHT
#define SPRITE_WIDTH    Constants::FIREBALL_SPRITE_WIDTH
#define SPRITE_HEIGHT   Constants::FIREBALL_SPRITE_HEIGHT

namespace sprite {

uint32 Fireball::attacking_animations_[8];
AnimationSet* Fireball::ANIMATIONS = NULL;

Fireball::Fireball(Vector2D &dir, Explosion *explosion) :
Projectile(0.0f, Constants::FIREBALL_SPEED, Constants::FIREBALL_DURATION, dir)
{

    ImageFactory *image_factory = WORLD()->image_factory();
    InitializeAnimations();
    Initialize( image_factory->FireballImage(), ANIMATIONS );
    set_hotspot(Vector2D(CENTER_X, CENTER_Y + SPRITE_HEIGHT));
    bound_ = new CircleObject(0.25f);
    set_light_radius(1.0f);


    float raw_angle = scene::World::FromWorldLinearCoordinates(dir).angle();
    float angle = (raw_angle / acos(-1.0f)) + 1.0f;
    int animation_index = (int)((angle * 4.0f) + 0.5f);
    this->SelectAnimation(animation_index % 8);


    explosion_ = explosion;
}

Fireball::~Fireball() {
}


void Fireball::Explode() {
    if (explosion_ != NULL) {
        WORLD()->AddWorldObject(explosion_, this->world_position());
        explosion_ = NULL;
    }
    Projectile::Explode();
}

void Fireball::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

void Fireball::Update(float delta_t) {
	Projectile::Update(delta_t);
   // this->Move(delta_t);
}

void Fireball::InitializeAnimations() {
    if(ANIMATIONS != NULL) return;
    ANIMATIONS = ugdk::Engine::reference()->animation_loader().Load("data/animations/fireball.gdd");
    attacking_animations_[0] = ANIMATIONS->MakeIndex("ATTACKING_LEFT");
    attacking_animations_[1] = ANIMATIONS->MakeIndex("ATTACKING_UP_LEFT");
    attacking_animations_[2] = ANIMATIONS->MakeIndex("ATTACKING_UP");
    attacking_animations_[3] = ANIMATIONS->MakeIndex("ATTACKING_UP_RIGHT");
    attacking_animations_[4] = ANIMATIONS->MakeIndex("ATTACKING_RIGHT");
    attacking_animations_[5] = ANIMATIONS->MakeIndex("ATTACKING_DOWN_RIGHT");
    attacking_animations_[6] = ANIMATIONS->MakeIndex("ATTACKING_DOWN");
    attacking_animations_[7] = ANIMATIONS->MakeIndex("ATTACKING_DOWN_LEFT");
}

}
