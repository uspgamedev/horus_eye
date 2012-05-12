#include "explosion.h"
#include <ugdk/base/engine.h>
#include <ugdk/base/resourcemanager.h>
#include <ugdk/action/animation.h>
#include <ugdk/action/animationset.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/spritesheet/flexiblespritesheet.h>
#include <ugdk/util/animationparser.h>
#include <ugdk/time/timeaccumulator.h>
#include <ugdk/graphic/light.h>

#include <pyramidworks/geometry/circle.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionlogic.h>

#include "game/scenes/world.h"
#include "game/components/mummy.h"
#include "game/components/damageable.h"
#include "game/utils/constants.h"

using namespace ugdk;
using namespace utils;
using component::Creature;

#define CENTER_X    Constants::EXPLOSION_SPRITE_CENTER_X
#define CENTER_Y    Constants::EXPLOSION_SPRITE_CENTER_Y
#define HEIGHT      Constants::EXPLOSION_HEIGHT
#define EXPLOSION_SPRITE_WIDTH    Constants::EXPLOSION_SPRITE_WIDTH
#define EXPLOSION_SPRITE_HEIGHT   Constants::EXPLOSION_SPRITE_HEIGHT

namespace sprite {

action::AnimationSet*   Explosion::ANIMATIONS = NULL;
const int       Explosion::HERO_FIREBALL_WEAPON = 0;
const int       Explosion::HERO_EXPLOSION_WEAPON = 1;
uint32          Explosion::WEAPON_ANIMATIONS[2];

COLLISION_DIRECT(double, DamageCollision, obj) {
    WorldObject *creature = (WorldObject *) obj;
    creature->damageable()->TakeDamage(data_);
}

Explosion::Explosion(ugdk::graphic::FlexibleSpritesheet *image, uint32 animation, double radius, double damage)
{

    ugdk::graphic::Sprite* sprite = new ugdk::graphic::Sprite(image, ANIMATIONS);
    node()->set_drawable(sprite);
    image->set_hotspot(Vector2D(CENTER_X, CENTER_Y));
    damage_ = damage;
    bound_ = new pyramidworks::geometry::Circle(radius / 2.0);
    set_light_radius(1.3*radius);

    Color light_color(1.0, 0.521568, 0.082352);
    node()->light()->set_color(light_color);

    sprite->AddObserverToAnimation(this);
    sprite->SelectAnimation(WEAPON_ANIMATIONS[animation]);

    expansion_speed_ = (radius / 2) /
            (sprite->GetAnimationFrameNumber() / sprite->GetAnimationFPS());

    INITIALIZE_COLLISION;
    SET_COLLISIONCLASS(Explosion);
    SET_COLLISIONSHAPE(bound_);
    ADD_COLLISIONLOGIC(Mummy, new DamageCollision(damage));

    //known_collisions_["Mummy"] = new Collisions::Damage(this);
}

Explosion::~Explosion() {}

void Explosion::InitializeAnimations() {
    ANIMATIONS = ugdk::base::ResourceManager::GetAnimationSetFromFile("animations/explosion.gdd");
    WEAPON_ANIMATIONS[HERO_FIREBALL_WEAPON] =
            ANIMATIONS->MakeIndex("HERO_FIREBALL_WEAPON");
    WEAPON_ANIMATIONS[HERO_EXPLOSION_WEAPON] =
            ANIMATIONS->MakeIndex("HERO_EXPLOSION_WEAPON");
}

void Explosion::ReleaseAnimations() {
    ANIMATIONS = NULL;
    WEAPON_ANIMATIONS[HERO_FIREBALL_WEAPON] = -1;
    WEAPON_ANIMATIONS[HERO_EXPLOSION_WEAPON] = -1;
}

void Explosion::Tick() {
    Die();
}

void Explosion::RadiusUpdate(double delta_t) {
    bound_->set_radius(bound_->radius() + expansion_speed_ * delta_t);
}

void Explosion::Update(double delta_t) {
    WorldObject::Update(delta_t);
    this->RadiusUpdate(delta_t);
}

void Explosion::set_world_position(const ugdk::Vector2D& pos) {
    super::set_world_position(pos);
    node()->set_zindex(node()->zindex() + node()->drawable()->height() * 0.1);
}

}
