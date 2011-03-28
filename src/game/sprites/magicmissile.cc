#include "magicmissile.h"
#include "mummy.h"
#include "../../framework/timeaccumulator.h"
#include "../utils/circleobject.h"
#include "../utils/constants.h"
#include "../utils/imagefactory.h"
#include "../scenes/world.h"

using namespace framework;
using namespace utils;

#define CENTER_X    Constants::PROJECTILE_SPRITE_CENTER_X
#define CENTER_Y    Constants::PROJECTILE_SPRITE_CENTER_Y
#define HEIGHT      Constants::PROJECTILE_HEIGHT
#define PROJECTILE_SPRITE_WIDTH    Constants::PROJECTILE_SPRITE_WIDTH
#define PROJECTILE_SPRITE_HEIGHT   Constants::PROJECTILE_SPRITE_HEIGHT

namespace sprite {

MagicMissile::MagicMissile(Vector2D & dir) :
        Projectile(Constants::PROJECTILE_DAMAGE, Constants::PROJECTILE_SPEED, Constants::PROJECTILE_DURATION, dir)
{
    ImageFactory *image_factory = WORLD()->image_factory();
    Initialize( image_factory->MagicMissileImage() );
    set_hotspot( Vector2D(CENTER_X, CENTER_Y + PROJECTILE_SPRITE_HEIGHT + HEIGHT) );
    bound_ = new CircleObject(0.15f);
    set_light_radius(1.0f);
}

MagicMissile::~MagicMissile() {}

void MagicMissile::CollidesWith(Mummy *obj) {
    if (this->status_ == WorldObject::STATUS_ACTIVE) {
        obj->TakeDamage(damage_);
    }
    Explode();
}

void MagicMissile::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

}
