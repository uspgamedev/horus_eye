#include "../../framework/audiomanager.h"
#include "mummyrangedweapon.h"
#include "../scenes/world.h"
#include "hero.h"
#include "mummyprojectile.h"

namespace sprite {

	void MummyRangedWeapon::Attack(){
		scene::World *world = WORLD();
		Hero* hero = world->hero();

		framework::Vector2D versor = framework::Vector2D::Normalized(hero->world_position() - owner_->world_position());
		framework::Vector2D pos = owner_->world_position();

		sprite::MummyProjectile * projectile = new sprite::MummyProjectile(pos, versor);
		world->AddWorldObject(projectile);
		Engine::reference()->audio_manager()->LoadSample("data/samples/fire.wav")->Play();
		
		((Mummy*)owner_)->StartAttack(hero);
	}

}
