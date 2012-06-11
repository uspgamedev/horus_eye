#include "mummyweapon.h"
#include "game/scenes/world.h"
#include "game/components/damageable.h"
#include "game/builders/explosionbuilder.h"

namespace sprite {
class Hero;
class Mummy;
}

namespace skills {

const double MummyWeapon::range_ = 1.0;

void MummyWeapon::Use(){
    super::Use();

	builder::ExplosionBuilder builder;
	sprite::WorldObject* punch = builder.MeleeExplosion(damage_,range_);

    scene::World *world = WORLD();
	world->AddWorldObject(punch, use_argument_.origin_);
}

bool MummyWeapon::IsValidUse() const {
    double distance = (use_argument_.destination_ - use_argument_.origin_).length();
    return (distance <= range_);
}

}
