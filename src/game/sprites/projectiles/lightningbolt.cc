#include <cmath>
#include <ugdk/time/timeaccumulator.h>
#include <ugdk/action/animation.h>
#include <ugdk/action/animationset.h>
#include <ugdk/util/animationparser.h>
#include <ugdk/base/engine.h>

#include "lightningbolt.h"

#include "game/sprites/mummy.h"
#include "game/utils/circleobject.h"
#include "game/utils/constants.h"
#include "game/utils/imagefactory.h"
#include "game/scenes/world.h"

using namespace ugdk;
using namespace utils;

#define CENTER_X    Constants::LIGHTNING_SPRITE_CENTER_X
#define CENTER_Y    Constants::LIGHTNING_SPRITE_CENTER_Y
#define HEIGHT      Constants::LIGHTNING_HEIGHT
#define SPRITE_WIDTH    Constants::LIGHTNING_SPRITE_WIDTH
#define SPRITE_HEIGHT   Constants::LIGHTNING_SPRITE_HEIGHT

namespace sprite {

uint32 LightningBolt::attacking_animations_[8];
AnimationSet* LightningBolt::ANIMATIONS = NULL;

LightningBolt::LightningBolt(Vector2D & dir) :
Projectile(Constants::LIGHTNING_DAMAGE, Constants::LIGHTNING_SPEED, Constants::LIGHTNING_DURATION, dir)
{
    ImageFactory *image_factory = WORLD()->image_factory();
    InitializeAnimations();
    Initialize( image_factory->LightningImage(), ANIMATIONS );
    set_hotspot(Vector2D(CENTER_X, CENTER_Y + SPRITE_HEIGHT));
    bound_ = new CircleObject(0.25f);
    set_light_radius(1.0f);


    float raw_angle = scene::World::FromWorldLinearCoordinates(dir).angle();
    float angle = (raw_angle / acos(-1.0f)) + 1.0f;
    int animation_index = (int)((angle * 4.0f) + 0.5f);
    this->SelectAnimation(animation_index % 8);

	known_collisions_[Mummy::Collision()] = new Collisions::Damage(this);
}

void LightningBolt::InitializeAnimations() {
    if(ANIMATIONS != NULL) return;
    ANIMATIONS = ugdk::Engine::reference()->animation_loader().Load("data/animations/lightning.gdd");
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
