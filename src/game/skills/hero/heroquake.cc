#include "game/skills/heroskills.h"

#include <ugdk/math/vector2D.h>
#include <ugdk/base/engine.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>

#include "game/builders/explosionbuilder.h"
#include "game/scenes/world.h"
#include "game/components/caster.h"
#include "game/utils/visionstrategy.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/utils/settings.h"
#include "game/skills/divinegift.h"

namespace skills {

using scene::World;
using utils::Constants;

class Quake : public DivineGift {
public:
    Quake(ugdk::graphic::Drawable* icon, SkillUseFunction use, double mana_cost, int block_cost)
      : DivineGift(icon, use, mana_cost, block_cost) {}

    virtual bool IsValidUse(const component::Caster* caster) const {
        utils::VisionStrategy vs;
        const usearguments::Aim& aim = caster->aim();
        double distance = (aim.destination_ - aim.origin_).length();
        return distance <= utils::Constants::QUAKE_EXPLOSION_RANGE 
            && vs.IsVisible(aim.destination_, aim.origin_);
    }
};

static void HeroQuakeUse(component::Caster* caster) {
    World *world = WORLD();
    builder::ExplosionBuilder builder;
    world->AddWorldObject(builder.EarthquakeExplosion(), caster->aim().destination_);

    if(utils::Settings::reference()->sound_effects())
        ugdk::Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

Skill* HeroQuake() {
    utils::HudImageFactory imfac;
    return new Quake(imfac.EarthquakeIconImage(), HeroQuakeUse, utils::Constants::QUAKE_COST,
        utils::Constants::QUAKE_BLOCK_COST);
}

} // namespace skills
