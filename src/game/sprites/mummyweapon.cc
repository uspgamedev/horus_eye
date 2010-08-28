#include "mummyweapon.h"
#include "../scenes/world.h"
#include "hero.h"

namespace sprite {

void MummyWeapon::Attack(){
	scene::World *world = WORLD();
	Hero* hero = world->hero();
	
	hero->TakeDamage();
	((Mummy*)owner_)->StartAttack(hero);
}

}
