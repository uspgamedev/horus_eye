#include "mummyweapon.h"
#include "game/scenes/world.h"
#include "game/sprites/creatures/hero.h"
#include "game/sprites/creatures/mummy.h"

namespace sprite {
class Hero;
class Mummy;
}

namespace skills {

void MummyWeapon::Use(){
	scene::World *world = WORLD();
	sprite::Hero* hero = world->hero();
	
	hero->TakeDamage(damage_);
}

}
