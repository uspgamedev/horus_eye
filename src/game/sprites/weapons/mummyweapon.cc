#include "mummyweapon.h"
#include "game/scenes/world.h"
#include "game/sprites/creatures/hero.h"
#include "game/sprites/mummy.h"

namespace sprite {

void MummyWeapon::Attack(){
	scene::World *world = WORLD();
	Hero* hero = world->hero();
	
	hero->TakeDamage(damage_);
	((Mummy*)owner_)->StartAttack(hero);
}

}
