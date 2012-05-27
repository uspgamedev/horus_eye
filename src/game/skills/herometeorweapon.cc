#include <ugdk/math/vector2D.h>
#include <ugdk/input/inputmanager.h>
#include <ugdk/audio/audiomanager.h>
#include <ugdk/action/animation.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>
#include <ugdk/graphic/node.h>
#include <ugdk/base/engine.h>

#include "herometeorweapon.h"

#include "game/builders/entitybuilder.h"
#include "game/scenes/world.h"
#include "game/components/logic/hero.h"
#include "game/sprites/worldobject.h"
#include "game/utils/visionstrategy.h"
#include "game/utils/imagefactory.h"
#include "game/utils/hudimagefactory.h"
#include "game/utils/constants.h"
#include "game/utils/settings.h"

namespace skills {

using namespace scene;
using namespace ugdk;
using namespace utils;
using utils::Constants;
using sprite::WorldObject;

class MeteorCarrier {
  protected:
    std::list<WorldObject*> drop_list_;
  public:
    MeteorCarrier(std::list<WorldObject*> &list) : drop_list_(list) {}
    MeteorCarrier(WorldObject *drop) { drop_list_.push_back(drop); }

    void operator()(WorldObject *wobj) {
        std::list<WorldObject*>::iterator it;
        scene::World* world = WORLD();
        for(it = drop_list_.begin(); it !=  drop_list_.end(); ++it)
            world->AddWorldObject(*it, wobj->world_position());
        drop_list_.clear();
    }
};

void HeroMeteorWeapon::Use(){
    super::Use();
    //TODO: use meteor's constants.

    World *world = WORLD();

    utils::ImageFactory *factory = world->image_factory();

    sprite::WorldObject *permanent_light = new sprite::WorldObject;
    permanent_light->set_light_radius(4.0);

    builder::EntityBuilder builder;
    sprite::WorldObject *explosion = builder.MeteorExplosion();

    std::list<sprite::WorldObject*> list;
    list.push_back(explosion);
    list.push_back(permanent_light);

    WorldObject* warning_effect = new WorldObject(3.0);
    warning_effect->set_death_start_callback(MeteorCarrier(list));

    world->AddWorldObject(warning_effect, use_argument_.destination_);

    if(utils::Settings::reference()->sound_effects())
        Engine::reference()->audio_manager()->LoadSample("samples/fire.wav")->Play();
}

HeroMeteorWeapon::HeroMeteorWeapon(component::Hero* owner)
    : DivineGift<usearguments::Aim>(NULL, utils::Constants::METEOR_COST, utils::Constants::METEOR_BLOCK_COST, owner->mana(), owner->mana_blocks(), owner->aim()) { // TODO: change cost
    HudImageFactory imfac;
    icon_ = imfac.LightningIconImage(); // TODO: change icon
}

bool HeroMeteorWeapon::IsValidUse() const {
    VisionStrategy vs;
    return CombatArt<usearguments::Aim>::IsValidUse() 
        && vs.IsVisible(use_argument_.destination_, use_argument_.origin_);
}

} // namespace skills
