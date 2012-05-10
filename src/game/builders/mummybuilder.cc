#include <ugdk/base/resourcemanager.h>
#include <ugdk/graphic/spritesheet/flexiblespritesheet.h>
#include <ugdk/graphic/node.h>

#include "mummybuilder.h"

#include "game/components/mummy.h"
#include "game/components/pharaoh.h"
#include "game/components/damageable.h"
#include "game/components/animation.h"
#include "game/components/graphic.h"
#include "game/utils/isometricanimationset.h"
#include "game/utils/constants.h"
#include "game/skills/mummyweapon.h"
#include "game/skills/mummyrangedweapon.h"
#include "game/skills/pharaohrangedweapon.h"
#include "game/skills/pharaohsummonweapon.h"
#include <game/resources/energy.h>

namespace builder {

using namespace sprite;
using namespace component;
using utils::Constants;
using resource::Energy;
using ugdk::Vector2D;

static utils::IsometricAnimationSet* ANIMATIONS = NULL;

MummyBuilder::MummyBuilder() {
    if(ANIMATIONS == NULL) {
        ANIMATIONS = new utils::IsometricAnimationSet(ugdk::base::ResourceManager::GetAnimationSetFromFile("animations/creature.gdd"));
    }
}

sprite::WorldObject* MummyBuilder::StandingMummy(ugdk::graphic::FlexibleSpritesheet *image) {
    WorldObject* wobj = new WorldObject;
    wobj->set_animation(new component::Animation(wobj->graphic(), image, ANIMATIONS));
    wobj->set_damageable(new component::Damageable(wobj));
    wobj->damageable()->life() = Energy(Constants::MUMMY_LIFE);

    Mummy* mummy = new Mummy(wobj, image);
	mummy->set_speed(Constants::MUMMY_SPEED);
	mummy->set_weapon(new skills::MummyWeapon(mummy, Constants::MUMMY_DAMAGE));
	mummy->set_bound(Constants::MUMMY_RADIUS);
    return wobj;
}

sprite::WorldObject* MummyBuilder::WalkingMummy(ugdk::graphic::FlexibleSpritesheet *image) {
	sprite::WorldObject* obj = StandingMummy(image);
    static_cast<Mummy*>(obj->logic())->set_standing(false);
	return obj;
}

sprite::WorldObject* MummyBuilder::StandingRangedMummy(ugdk::graphic::FlexibleSpritesheet *image) {
    WorldObject* wobj = new WorldObject;
    wobj->set_animation(new component::Animation(wobj->graphic(), image, ANIMATIONS));
    wobj->set_damageable(new component::Damageable(wobj));
    wobj->damageable()->life() = Energy(Constants::RANGED_MUMMY_LIFE);

    Mummy* mummy = new Mummy(wobj, image);
	mummy->set_speed(Constants::MUMMY_SPEED);
	mummy->set_weapon(new skills::MummyRangedWeapon(mummy, Constants::RANGED_MUMMY_DAMAGE));
	mummy->set_bound(Constants::MUMMY_RADIUS);
	return wobj;
}

sprite::WorldObject* MummyBuilder::RangedMummy(ugdk::graphic::FlexibleSpritesheet *image) {
	sprite::WorldObject* obj = StandingRangedMummy(image);
    static_cast<Mummy*>(obj->logic())->set_standing(false);
	return obj;
}

sprite::WorldObject* MummyBuilder::StandingBigMummy(ugdk::graphic::FlexibleSpritesheet *image) {
    WorldObject* wobj = new WorldObject;
    wobj->set_animation(new component::Animation(wobj->graphic(), image, ANIMATIONS));
    wobj->set_damageable(new component::Damageable(wobj));
    wobj->node()->modifier()->set_scale(Vector2D(2.0, 2.0));
    wobj->damageable()->life() = Energy(Constants::BIG_MUMMY_LIFE);
    wobj->damageable()->set_super_armor(true);
    Mummy *mummy = new Mummy(wobj, image);
    mummy->set_speed(Constants::BIG_MUMMY_SPEED);
	mummy->set_weapon(new skills::MummyWeapon(mummy, Constants::BIG_MUMMY_DAMAGE));
    mummy->set_bound(Constants::BIG_MUMMY_RADIUS);
    return wobj;
}

sprite::WorldObject * MummyBuilder::BigMummy(ugdk::graphic::FlexibleSpritesheet *image) {
	sprite::WorldObject* obj = StandingBigMummy(image);
    static_cast<Mummy*>(obj->logic())->set_standing(false);
	return obj;
}

sprite::WorldObject * MummyBuilder::StandingPharaoh(ugdk::graphic::FlexibleSpritesheet *image) {
	Pharaoh *pharaoh = new Pharaoh(new WorldObject, image, Constants::PHARAOH_LIFE, Constants::PHARAOH_MANA);
    pharaoh->set_speed(Constants::PHARAOH_SPEED);
	pharaoh->set_weapon(new skills::MummyWeapon(pharaoh, Constants::PHARAOH_DAMAGE));
	pharaoh->set_ranged_weapon(new skills::PharaohRangedWeapon(pharaoh, Constants::PHARAOH_RANGED_DAMAGE));
	pharaoh->set_summon_weapon(new skills::PharaohSummonWeapon(pharaoh));
    pharaoh->set_bound(Constants::PHARAOH_RADIUS);
	return pharaoh->owner();
}

sprite::WorldObject * MummyBuilder::WalkingPharaoh(ugdk::graphic::FlexibleSpritesheet *image) {
	sprite::WorldObject* obj = StandingPharaoh(image);
    static_cast<Mummy*>(obj->logic())->set_standing(false);
	return obj;
}

} // namespace builder
