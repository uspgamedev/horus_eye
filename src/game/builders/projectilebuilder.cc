#include <cmath>
#include <ugdk/base/engine.h>
#include <ugdk/action/animationset.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/base/types.h>
#include <ugdk/graphic/light.h>
#include <pyramidworks/geometry/circle.h>

#include "projectilebuilder.h"

#include "game/sprites/creatures/creature.h"
#include "game/sprites/explosion.h"
#include "game/sprites/projectiles/carrier.h"
#include "game/utils/constants.h"
#include "game/utils/imagefactory.h"
#include "game/scenes/world.h"

namespace ugdk {
class Image;
}

namespace builder {

using namespace sprite;
using ugdk::Vector2D;
using utils::Constants;


static int GetAnimationIndexFromDir(Vector2D &dir) {
    float raw_angle = scene::World::FromWorldLinearCoordinates(dir).angle();
    float angle = (raw_angle / acos(-1.0f)) + 1.0f;
    int animation_index = (int)((angle * 4.0f) + 0.5f);
    return (animation_index % 8);
}

ugdk::AnimationSet* ProjectileBuilder::fireball_animation_ = NULL;
ugdk::AnimationSet* ProjectileBuilder::lightning_animation_ = NULL;
ugdk::uint32 ProjectileBuilder::fireball_animation_map_[8], 
             ProjectileBuilder::lightning_animation_map_[8];

struct ObjectAndDamage {
    ObjectAndDamage(WorldObject *object, float dmg)
        : obj(object), damage(dmg) {}

    WorldObject *obj;
    float damage;
};

COLLISION_DIRECT(WorldObject*, DieCollision, data) { 
    data_->Die();
}

COLLISION_DIRECT(float, DamageCollision, obj) {
	Creature *creature = (Creature *) obj;
    creature->TakeDamage(data_);
}

COLLISION_DIRECT(struct ObjectAndDamage, DamageAndDieCollision, obj) {
	Creature *creature = (Creature *) obj;
    if (data_.obj->is_active())
        creature->TakeDamage(data_.damage);
    data_.obj->Die();
}

static CollisionObject* buildBasicCollision(Projectile* proj, float radius) {
    CollisionObject* col = new CollisionObject(proj);
    col->InitializeCollisionClass(GET_COLLISIONMASK(Projectile));
    col->AddCollisionLogic(GET_COLLISIONMASK(Wall), new DieCollision(proj));
    proj->set_collision_object(col);
    proj->set_shape(new pyramidworks::geometry::Circle(radius));
    return col;
}

void ProjectileBuilder::InitializeAnimations() {
    if(fireball_animation_ == NULL) {
        fireball_animation_ = ugdk::Engine::reference()->animation_loader().Load("animations/fireball.gdd");
        fireball_animation_map_[0] = fireball_animation_->MakeIndex("ATTACKING_LEFT");
        fireball_animation_map_[1] = fireball_animation_->MakeIndex("ATTACKING_UP_LEFT");
        fireball_animation_map_[2] = fireball_animation_->MakeIndex("ATTACKING_UP");
        fireball_animation_map_[3] = fireball_animation_->MakeIndex("ATTACKING_UP_RIGHT");
        fireball_animation_map_[4] = fireball_animation_->MakeIndex("ATTACKING_RIGHT");
        fireball_animation_map_[5] = fireball_animation_->MakeIndex("ATTACKING_DOWN_RIGHT");
        fireball_animation_map_[6] = fireball_animation_->MakeIndex("ATTACKING_DOWN");
        fireball_animation_map_[7] = fireball_animation_->MakeIndex("ATTACKING_DOWN_LEFT");
    }

    if(lightning_animation_ == NULL) {
        lightning_animation_ = ugdk::Engine::reference()->animation_loader().Load("animations/lightning.gdd");
        lightning_animation_map_[0] = lightning_animation_->MakeIndex("ATTACKING_LEFT");
        lightning_animation_map_[1] = lightning_animation_->MakeIndex("ATTACKING_UP_LEFT");
        lightning_animation_map_[2] = lightning_animation_->MakeIndex("ATTACKING_UP");
        lightning_animation_map_[3] = lightning_animation_->MakeIndex("ATTACKING_UP_RIGHT");
        lightning_animation_map_[4] = lightning_animation_->MakeIndex("ATTACKING_RIGHT");
        lightning_animation_map_[5] = lightning_animation_->MakeIndex("ATTACKING_DOWN_RIGHT");
        lightning_animation_map_[6] = lightning_animation_->MakeIndex("ATTACKING_DOWN");
        lightning_animation_map_[7] = lightning_animation_->MakeIndex("ATTACKING_DOWN_LEFT");
    }
}

Projectile* ProjectileBuilder::MagicMissile(Vector2D &dir) {
    factory_->MagicMissileImage()->set_hotspot( Vector2D(Constants::PROJECTILE_SPRITE_CENTER_X, Constants::PROJECTILE_SPRITE_CENTER_Y + Constants::PROJECTILE_SPRITE_HEIGHT + Constants::PROJECTILE_HEIGHT) );

    Projectile *proj = new Projectile(Constants::PROJECTILE_SPEED, Constants::PROJECTILE_DURATION, dir);
    proj->node()->set_drawable(new ugdk::Sprite( factory_->MagicMissileImage()));
    proj->set_light_radius(1.0f);

    CollisionObject* col = buildBasicCollision(proj, 0.15f);
    struct ObjectAndDamage data(proj, Constants::PROJECTILE_DAMAGE);
    col->AddCollisionLogic(GET_COLLISIONMASK(Mummy), new DamageAndDieCollision(data));
    return proj;
}

Projectile* ProjectileBuilder::MummyProjectile(Vector2D &dir, int damage) {
    Projectile *proj = new Projectile(Constants::PROJECTILE_SPEED, Constants::PROJECTILE_DURATION, dir);
    proj->node()->set_drawable(new ugdk::Sprite( factory_->MummyProjectileImage() ));
    proj->node()->drawable()->set_hotspot(Vector2D(0.0f, Constants::PROJECTILE_SPRITE_HEIGHT + Constants::PROJECTILE_HEIGHT));
    proj->set_light_radius(0.75f);

    CollisionObject* col = buildBasicCollision(proj, 0.15f);
    struct ObjectAndDamage data(proj, damage);
    col->AddCollisionLogic(GET_COLLISIONMASK(Hero), new DamageAndDieCollision(data));
    return proj;
}

Projectile* ProjectileBuilder::LightningBolt(Vector2D &dir) {
    factory_->LightningImage()->set_hotspot(Vector2D(Constants::LIGHTNING_SPRITE_CENTER_X, Constants::LIGHTNING_SPRITE_CENTER_Y + Constants::LIGHTNING_SPRITE_HEIGHT));

    Projectile *proj = new Projectile(Constants::LIGHTNING_SPEED, Constants::LIGHTNING_DURATION, dir);
    ugdk::Sprite* sprite = new ugdk::Sprite( factory_->LightningImage(), lightning_animation_ );
    sprite->SelectAnimation(GetAnimationIndexFromDir(dir));
    proj->node()->set_drawable(sprite);
    proj->set_light_radius(1.0f);


    CollisionObject* col = buildBasicCollision(proj, 0.25f);
    col->AddCollisionLogic(GET_COLLISIONMASK(Mummy), new DamageCollision(Constants::LIGHTNING_DAMAGE));
    return proj;
}

Projectile* ProjectileBuilder::Fireball(Vector2D &dir) {
    factory_->FireballImage()->set_hotspot(Vector2D(Constants::FIREBALL_SPRITE_CENTER_X, Constants::FIREBALL_SPRITE_CENTER_Y + Constants::FIREBALL_SPRITE_HEIGHT));

    Explosion *explosion = new Explosion(factory_->ExplosionImage(), 
        Explosion::HERO_FIREBALL_WEAPON, Constants::FIREBALL_EXPLOSION_RADIUS, Constants::FIREBALL_EXPLOSION_DAMAGE);
    Projectile *proj = new Carrier(Constants::FIREBALL_SPEED, Constants::FIREBALL_DURATION, dir, explosion);
    ugdk::Sprite* sprite = new ugdk::Sprite( factory_->FireballImage(), fireball_animation_ );
    sprite->SelectAnimation(GetAnimationIndexFromDir(dir));
    proj->node()->set_drawable(sprite);
    proj->set_light_radius(1.0f);

    // Give the light an orange color
    ugdk::Color light_color(1.0f, 0.521568f, 0.082352f);
    proj->node()->light()->set_color(light_color);


    CollisionObject* col = buildBasicCollision(proj, 0.25f);
    col->AddCollisionLogic(GET_COLLISIONMASK(Mummy), new DieCollision(proj));
    return proj;
}

}
