#include "papermummyweapon.h"
#include "game/scenes/world.h"
#include "game/components/animation.h"
#include "game/components/damageable.h"

namespace sprite {
class Hero;
class Mummy;
}

namespace skills {

const double PaperMummyWeapon::range_ = 1.0;

void PaperMummyWeapon::Use(){
    super::Use();

    scene::World *world = WORLD();
    sprite::WorldObject* hero = world->hero();
    if(hero && hero->damageable())
        hero->damageable()->TakeDamage(damage_);
    owner_->owner()->damageable()->TakeDamage(1.0);
}

bool PaperMummyWeapon::IsValidUse() const {
    double distance = (use_argument_.destination_ - use_argument_.origin_).length();
    return (distance <= range_);
}

}
