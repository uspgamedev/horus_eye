#include <ugdk/math/vector2D.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/action/animation.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/node.h>
#include <ugdk/base/engine.h>

#include "herometeorweapon.h"

#include "game/builders/explosionbuilder.h"
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"
#include "game/utils/visionstrategy.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/utils/settings.h"
#include "game/builders/functions/carrier.h"

namespace skills {

using namespace scene;
using namespace ugdk;
using namespace utils;
using utils::Constants;
using sprite::WorldObject;

void HeroMeteorWeapon::Use(){
    super::Use();
    //TODO: use meteor's constants.

    World *world = WORLD();

    sprite::WorldObject *permanent_light = new sprite::WorldObject;
    permanent_light->set_light_radius(4.0);

    builder::ExplosionBuilder builder;
    sprite::WorldObject *explosion = builder.MeteorExplosion();

    std::list<sprite::WorldObject*> list;
    list.push_back(explosion);
    list.push_back(permanent_light);

    WorldObject* warning_effect = new WorldObject(3.0);
    warning_effect->set_start_to_die_callback(builder::function::Carrier(list));

    world->AddWorldObject(warning_effect, use_argument_.destination_);

    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

HeroMeteorWeapon::HeroMeteorWeapon(component::Caster* caster)
    : DivineGift<usearguments::Aim>(NULL, utils::Constants::METEOR_COST,
    		utils::Constants::METEOR_BLOCK_COST, caster->mana(), caster->mana_blocks(), caster->aim()) {
    HudImageFactory imfac;
    icon_ = imfac.MeteorIconImage(); // TODO: change icon
}

bool HeroMeteorWeapon::IsValidUse() const {
    VisionStrategy vs;
    return CombatArt<usearguments::Aim>::IsValidUse() 
        && vs.IsVisible(use_argument_.destination_, use_argument_.origin_);
}

} // namespace skills
