#include "mummyweapon.h"
#include "game/scenes/world.h"
#include "game/components/damageable.h"
#include "game/components/mummy.h"

namespace sprite {
class Hero;
class Mummy;
}

namespace skills {

const double MummyWeapon::range_ = 1.0;

void MummyWeapon::Use(){
    super::Use();

	scene::World *world = WORLD();
	sprite::WorldObject* hero = world->hero();
    if(hero && hero->damageable())
        hero->damageable()->TakeDamage(damage_);
}

bool MummyWeapon::IsValidUse() const {
    double distance = (use_argument_.destination_ - use_argument_.origin_).length();
    return (distance <= range_);
}

}
