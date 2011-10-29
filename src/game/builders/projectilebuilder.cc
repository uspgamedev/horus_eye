#include <pyramidworks/geometry/circle.h>

#include "projectilebuilder.h"

#include "game/sprites/creatures/creature.h"
#include "game/sprites/explosion.h"
#include "game/utils/constants.h"
#include "game/utils/imagefactory.h"

namespace ugdk {
class Image;
}

namespace builder {

using namespace sprite;
using ugdk::Vector2D;
using utils::Constants;

COLLISION_DIRECT(Projectile, Explode, data) { 
    owner_->Explode();
}

COLLISION_DIRECT(Projectile, Damage, obj) {
	Creature *creature = (Creature *) obj;
    if (owner_->status() == WorldObject::STATUS_ACTIVE)
        creature->TakeDamage(owner_->damage());
}

COLLISION_DIRECT(Projectile, DamageAndExplode, obj) {
	Creature *creature = (Creature *) obj;
    if (owner_->status() == WorldObject::STATUS_ACTIVE)
        creature->TakeDamage(owner_->damage());
    owner_->Explode();
}

Projectile* ProjectileBuilder::MagicMissile(Vector2D &dir) {
    Projectile *proj = new Projectile(Constants::PROJECTILE_DAMAGE, Constants::PROJECTILE_SPEED, Constants::PROJECTILE_DURATION, dir);
    proj->Initialize( factory_->MagicMissileImage() );
    proj->set_hotspot( Vector2D(Constants::PROJECTILE_SPRITE_CENTER_X, 
        Constants::PROJECTILE_SPRITE_CENTER_Y + Constants::PROJECTILE_SPRITE_HEIGHT + Constants::PROJECTILE_HEIGHT) );
    proj->set_light_radius(1.0f);

    OBJADD_COLLISIONGEOM(proj, Projectile, new pyramidworks::geometry::Circle(0.15f));
    OBJADD_COLLISIONLOGIC(proj, Mummy, new DamageAndExplode(proj));
    return proj;
}

Projectile* ProjectileBuilder::MummyProjectile(Vector2D &dir, int damage) {
    Projectile *proj = new Projectile(damage, Constants::PROJECTILE_SPEED, Constants::PROJECTILE_DURATION, dir);
    proj->Initialize( factory_->MummyProjectileImage() );
    proj->set_hotspot( Vector2D(Constants::PROJECTILE_SPRITE_CENTER_X, 
        Constants::PROJECTILE_SPRITE_CENTER_Y + Constants::PROJECTILE_SPRITE_HEIGHT + Constants::PROJECTILE_HEIGHT) );
    proj->set_light_radius(0.75f);

    OBJADD_COLLISIONGEOM(proj, Projectile, new pyramidworks::geometry::Circle(0.15f));
    OBJADD_COLLISIONLOGIC(proj, Hero, new DamageAndExplode(proj));
    return proj;
}

Projectile* ProjectileBuilder::LightningBolt(Vector2D &dir) {
    return MagicMissile(dir);
}

Projectile* ProjectileBuilder::Fireball(Vector2D &dir) {
    //Explosion *explosion = new Explosion(factory_->ExplosionImage(), 
    //    Explosion::HERO_FIREBALL_WEAPON, Constants::FIREBALL_EXPLOSION_RADIUS, Constants::FIREBALL_EXPLOSION_DAMAGE);
    return MagicMissile(dir);
}

}
