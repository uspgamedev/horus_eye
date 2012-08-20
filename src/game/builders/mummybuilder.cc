#include <ugdk/base/resourcemanager.h>
#include <ugdk/base/types.h>
#include <ugdk/graphic/node.h>

#include "mummybuilder.h"

#include "game/components/logic/mummy.h"
#include "game/components/logic/pharaoh.h"
#include "game/components/damageable.h"
#include "game/components/animation.h"
#include "game/components/graphic.h"
#include "game/utils/imagefactory.h"
#include "game/utils/isometricanimationset.h"
#include "game/utils/constants.h"
#include "game/skills/mummyweapon.h"
#include "game/skills/mummyrangedweapon.h"
#include "game/skills/papermummyweapon.h"
#include "game/skills/pharaohrangedweapon.h"
#include "game/skills/pharaohsummonweapon.h"
#include <game/resources/energy.h>

namespace builder {
namespace MummyBuilder {

using namespace sprite;
using namespace component;
using utils::Constants;
using resource::Energy;
using ugdk::Vector2D;

static utils::IsometricAnimationSet* ANIMATIONS = NULL;

static WorldObject* build_mummy_wobj(const std::string& tag, double life) {
    if(ANIMATIONS == NULL) {
        ANIMATIONS = new utils::IsometricAnimationSet(ugdk::base::ResourceManager::GetAnimationSetFromFile("animations/creature.gdd"));
    }
    WorldObject* wobj = new WorldObject;
    wobj->set_animation(new component::Animation(wobj, tag, ANIMATIONS));
    wobj->set_damageable(new component::Damageable(wobj, 300));
    wobj->damageable()->life() = Energy(life);
    wobj->animation()->AddCallback(utils::DYING, &WorldObject::Die);
    return wobj;
}

sprite::WorldObject* StandingMummy(const std::vector<std::string>& arguments) {
    utils::ImageFactory factory;
    WorldObject* wobj = build_mummy_wobj("mummy_basic", Constants::MUMMY_LIFE);

    Mummy* mummy = new Mummy(wobj);
    mummy->set_speed(Constants::MUMMY_SPEED);
    mummy->set_weapon(new skills::MummyWeapon(mummy, Constants::MUMMY_DAMAGE));
    mummy->set_bound(Constants::MUMMY_RADIUS);
    return wobj;
}

sprite::WorldObject* WalkingMummy(const std::vector<std::string>& arguments) {
    utils::ImageFactory factory;
    sprite::WorldObject* obj = StandingMummy(arguments);
    static_cast<Mummy*>(obj->logic())->set_standing(false);
    return obj;
}

sprite::WorldObject* StandingRangedMummy(const std::vector<std::string>& arguments) {
    utils::ImageFactory factory;
    WorldObject* wobj = build_mummy_wobj("mummy_ranged", Constants::RANGED_MUMMY_LIFE);

    Mummy* mummy = new Mummy(wobj);
    mummy->set_speed(Constants::MUMMY_SPEED);
    mummy->set_weapon(new skills::MummyRangedWeapon(mummy, Constants::RANGED_MUMMY_DAMAGE));
    mummy->set_bound(Constants::MUMMY_RADIUS);
    return wobj;
}

sprite::WorldObject* WalkingRangedMummy(const std::vector<std::string>& arguments) {
    sprite::WorldObject* obj = StandingRangedMummy(arguments);
    static_cast<Mummy*>(obj->logic())->set_standing(false);
    return obj;
}

sprite::WorldObject* StandingBigMummy(const std::vector<std::string>& arguments) {
    utils::ImageFactory factory;
    WorldObject* wobj = build_mummy_wobj("mummy_big", Constants::BIG_MUMMY_LIFE);
    wobj->node()->modifier()->set_scale(Vector2D(2.0, 2.0));
    wobj->damageable()->set_super_armor(true);

    Mummy *mummy = new Mummy(wobj);
    mummy->set_speed(Constants::BIG_MUMMY_SPEED);
    mummy->set_weapon(new skills::MummyWeapon(mummy, Constants::BIG_MUMMY_DAMAGE));
    mummy->set_bound(Constants::BIG_MUMMY_RADIUS);
    return wobj;
}

sprite::WorldObject * WalkingBigMummy(const std::vector<std::string>& arguments) {
    sprite::WorldObject* obj = StandingBigMummy(arguments);
    static_cast<Mummy*>(obj->logic())->set_standing(false);
    return obj;
}

sprite::WorldObject *StandingPaperMummy(const std::vector<std::string>& arguments) {
    utils::ImageFactory factory;
    WorldObject* wobj = build_mummy_wobj("mummy_basic", Constants::PAPER_MUMMY_LIFE);
    ugdk::Color color = wobj->graphic()->node()->modifier()->color();
    color.set_a(0.5);
    wobj->graphic()->node()->modifier()->set_color(color);

    Mummy* mummy = new Mummy(wobj);
    mummy->set_speed(Constants::MUMMY_SPEED);
    mummy->set_weapon(new skills::PaperMummyWeapon(mummy, Constants::PAPER_MUMMY_DAMAGE));
    mummy->set_bound(Constants::MUMMY_RADIUS);
    return wobj;
}

sprite::WorldObject *WalkingPaperMummy(const std::vector<std::string>& arguments) {
    utils::ImageFactory factory;
    sprite::WorldObject* obj = StandingPaperMummy(arguments);
    static_cast<Mummy*>(obj->logic())->set_standing(false);
    return obj;
}

sprite::WorldObject * StandingPharaoh(const std::vector<std::string>& arguments) {
    utils::ImageFactory factory;
    WorldObject* wobj = build_mummy_wobj("pharaoh", Constants::PHARAOH_LIFE);
    wobj->damageable()->set_super_armor(true);
    Pharaoh *pharaoh = new Pharaoh(wobj, Constants::PHARAOH_MANA);
    pharaoh->set_speed(Constants::PHARAOH_SPEED);
    pharaoh->set_weapon(new skills::MummyWeapon(pharaoh, Constants::PHARAOH_DAMAGE));
    pharaoh->set_ranged_weapon(new skills::PharaohRangedWeapon(pharaoh, Constants::PHARAOH_RANGED_DAMAGE));
    pharaoh->set_summon_weapon(new skills::PharaohSummonWeapon(pharaoh));
    pharaoh->set_bound(Constants::PHARAOH_RADIUS);
    return wobj;
}

sprite::WorldObject * WalkingPharaoh(const std::vector<std::string>& arguments) {
    sprite::WorldObject* obj = StandingPharaoh(arguments);
    static_cast<Mummy*>(obj->logic())->set_standing(false);
    return obj;
}

} // namespace MummyBuilder

} // namespace builder
