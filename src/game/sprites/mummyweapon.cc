#include "mummyweapon.h"
#include "../scenes/world.h"
#include "hero.h"

namespace sprite {

void MummyWeapon::Attack(Vector2D target){
	scene::World *world = ((scene::World *)Engine::reference()->CurrentScene());
	Hero* hero = world->hero();
	
	hero->TakeDamage();
	((Mummy*)owner_)->StartAttack(hero);
}

}
