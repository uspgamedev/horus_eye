#include "game/skills/heroskills.h"

#include <ugdk/math/vector2D.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/base/engine.h>

#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"
#include "game/utils/constants.h"
#include "game/utils/visionstrategy.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/utils/settings.h"
#include "game/components/caster.h"

namespace skills {

using namespace scene;
using namespace ugdk;
using namespace utils;
using utils::Constants;

class Light : public CombatArt {
public:
    Light(ugdk::graphic::Drawable* icon, SkillUseFunction use, double mana_cost)
      : CombatArt(icon, use, mana_cost) {}

    virtual bool IsValidUse(const component::Caster* caster) const {
        VisionStrategy vs;
        return vs.IsVisible(caster->aim().destination_, caster->aim().origin_);
    }
};

static void HeroLightUse(component::Caster* caster){
    World *world = WORLD();   

    sprite::WorldObject *light = new sprite::WorldObject(5.0);
    light->node()->set_drawable(new ugdk::graphic::Sprite("light"));
    light->set_light_radius(4.0);
    world->AddWorldObject(light, caster->aim().destination_);

    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

Skill* HeroLightning() {
    HudImageFactory imfac;
    return new Light(imfac.LightIconImage(), HeroLightUse, utils::Constants::QUAKE_COST);
}

} // namespace skills
