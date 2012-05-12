#include <cmath>
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/action/animationset.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/base/types.h>
#include <ugdk/graphic/light.h>
#include <ugdk/graphic/node.h>
#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>
#include <pyramidworks/collision/collisionlogic.h>

#include "projectilebuilder.h"

#include "game/components/damageable.h"
#include "game/components/projectile.h"
#include "game/sprites/explosion.h"
#include "game/utils/constants.h"
#include "game/utils/imagefactory.h"
#include "game/scenes/world.h"

namespace builder {

using namespace sprite;
using component::Projectile;
using ugdk::Vector2D;
using utils::Constants;
using sprite::WorldObject;
using pyramidworks::collision::CollisionObject;

class Carrier {
  protected:
    std::list<WorldObject*> drop_list_;
  public:
    Carrier(std::list<WorldObject*> &list) : drop_list_(list) {}
    Carrier(WorldObject *drop) { drop_list_.push_back(drop); }

    void operator()(WorldObject *wobj) {
        std::list<WorldObject*>::iterator it;
        scene::World* world = WORLD();
        for(it = drop_list_.begin(); it !=  drop_list_.end(); ++it)
            world->AddWorldObject(*it, wobj->world_position());
        drop_list_.clear();
        wobj->Die();
    }
};

static int GetAnimationIndexFromDir(Vector2D &dir) {
    double raw_angle = scene::World::FromWorldLinearCoordinates(dir).angle();
    double angle = (raw_angle / acos(-1.0)) + 1.0;
    int animation_index = (int)((angle * 4.0) + 0.5);
    return (animation_index % 8);
}

ugdk::action::AnimationSet* ProjectileBuilder::fireball_animation_ = NULL;
ugdk::action::AnimationSet* ProjectileBuilder::lightning_animation_ = NULL;
ugdk::uint32 ProjectileBuilder::fireball_animation_map_[8], 
             ProjectileBuilder::lightning_animation_map_[8];

struct ObjectAndDamage {
    ObjectAndDamage(WorldObject *object, double dmg)
        : obj(object), damage(dmg) {}

    WorldObject *obj;
    double damage;
};

COLLISION_DIRECT(WorldObject*, DieCollision, data) { 
    data_->StartToDie();
}

COLLISION_DIRECT(double, DamageCollision, obj) {
    WorldObject *wobj = (WorldObject *) obj;
    wobj->damageable()->TakeDamage(data_);
}

COLLISION_DIRECT(struct ObjectAndDamage, DamageAndDieCollision, obj) {
    WorldObject *wobj = static_cast<WorldObject*>(obj);
    if (data_.obj->is_active())
        wobj->damageable()->TakeDamage(data_.damage);
    data_.obj->StartToDie();
}

static WorldObject* buildObject(double duration, double radius) {
    WorldObject* wobj = new WorldObject(Constants::PROJECTILE_DURATION);

    CollisionObject* col = new CollisionObject(WORLD()->collision_manager(), wobj);
    col->InitializeCollisionClass("Projectile");
    col->AddCollisionLogic("Wall", new DieCollision(wobj));
    wobj->set_collision_object(col);
    wobj->set_shape(new pyramidworks::geometry::Circle(radius));
    return wobj;
}

void ProjectileBuilder::InitializeAnimations() {
    if(fireball_animation_ == NULL) {
        fireball_animation_ = ugdk::base::ResourceManager::GetAnimationSetFromFile("animations/fireball.gdd");
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
        lightning_animation_ = ugdk::base::ResourceManager::GetAnimationSetFromFile("animations/lightning.gdd");
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

WorldObject* ProjectileBuilder::MagicMissile(Vector2D &dir) {
    factory_->MagicMissileImage()->set_hotspot( Vector2D(Constants::PROJECTILE_SPRITE_CENTER_X, Constants::PROJECTILE_SPRITE_CENTER_Y + Constants::PROJECTILE_SPRITE_HEIGHT + Constants::PROJECTILE_HEIGHT) );

    WorldObject* wobj = buildObject(Constants::PROJECTILE_DURATION, 0.15);
    wobj->node()->set_drawable(new ugdk::graphic::Sprite( factory_->MagicMissileImage()));
    wobj->set_light_radius(1.0);

    Projectile *proj = new Projectile(wobj, Constants::PROJECTILE_SPEED, dir);
    wobj->set_logic(proj);

    struct ObjectAndDamage data(wobj, Constants::PROJECTILE_DAMAGE);
    wobj->collision_object()->AddCollisionLogic("Mummy", new DamageAndDieCollision(data));
    return wobj;
}

sprite::WorldObject* ProjectileBuilder::MummyProjectile(ugdk::Vector2D &dir, int damage) {
    WorldObject* wobj = buildObject(Constants::PROJECTILE_DURATION, 0.15);
    wobj->node()->set_drawable(new ugdk::graphic::Sprite( factory_->MummyProjectileImage() ));
    wobj->node()->drawable()->set_hotspot(Vector2D(0.0, Constants::PROJECTILE_SPRITE_HEIGHT + Constants::PROJECTILE_HEIGHT));
    wobj->set_light_radius(0.75);

    Projectile *proj = new Projectile(wobj, Constants::PROJECTILE_SPEED, dir);
    wobj->set_logic(proj);

    struct ObjectAndDamage data(wobj, damage);
    wobj->collision_object()->AddCollisionLogic("Hero", new DamageAndDieCollision(data));

    return wobj;
}

WorldObject* ProjectileBuilder::LightningBolt(Vector2D &dir) {
    factory_->LightningImage()->set_hotspot(Vector2D(Constants::LIGHTNING_SPRITE_CENTER_X, Constants::LIGHTNING_SPRITE_CENTER_Y + Constants::LIGHTNING_SPRITE_HEIGHT));

    WorldObject* wobj = buildObject(Constants::LIGHTNING_DURATION, 0.25);

    ugdk::graphic::Sprite* sprite = new ugdk::graphic::Sprite( factory_->LightningImage(), lightning_animation_ );
    sprite->SelectAnimation(GetAnimationIndexFromDir(dir));
    wobj->node()->set_drawable(sprite);
    wobj->set_light_radius(1.0);
    wobj->set_logic(new Projectile(wobj, Constants::LIGHTNING_SPEED, dir));
    wobj->collision_object()->AddCollisionLogic("Mummy", new DamageCollision(Constants::LIGHTNING_DAMAGE));
    return wobj;
}

WorldObject* ProjectileBuilder::Fireball(Vector2D &dir) {
    factory_->FireballImage()->set_hotspot(Vector2D(Constants::FIREBALL_SPRITE_CENTER_X, Constants::FIREBALL_SPRITE_CENTER_Y + Constants::FIREBALL_SPRITE_HEIGHT));

    Explosion *explosion = new Explosion(factory_->ExplosionImage(), 
        Explosion::HERO_FIREBALL_WEAPON, Constants::FIREBALL_EXPLOSION_RADIUS, Constants::FIREBALL_EXPLOSION_DAMAGE);

    WorldObject* wobj = buildObject(Constants::FIREBALL_DURATION, 0.25);

    ugdk::graphic::Sprite* sprite = new ugdk::graphic::Sprite( factory_->FireballImage(), fireball_animation_ );
    sprite->SelectAnimation(GetAnimationIndexFromDir(dir));
    wobj->node()->set_drawable(sprite);
    wobj->set_light_radius(1.0);

    // Give the light an orange color
    ugdk::Color light_color(1.0, 0.521568, 0.082352);
    wobj->node()->light()->set_color(light_color);

    wobj->set_death_start_callback(Carrier(explosion));
    wobj->set_logic(new Projectile(wobj, Constants::FIREBALL_SPEED, dir));
    wobj->collision_object()->AddCollisionLogic("Mummy", new DieCollision(wobj));
    return wobj;
}

}
