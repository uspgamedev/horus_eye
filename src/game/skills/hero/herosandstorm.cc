#include "game/skills/heroskills.h"

#include <ugdk/base/engine.h>
#include <ugdk/graphic/drawable/texturedrectangle.h>

#include "game/map/room.h"
#include "game/utils/settings.h"
#include "game/entities/sandstormemitter.h"
#include "game/utils/imagefactory.h"
#include "game/utils/hudimagefactory.h"
#include "game/components/caster.h"
#include "game/skills/combatart.h"

namespace skills {

using ugdk::Engine;
using scene::World;
using utils::Constants;
using entities::SandstormEmitter;
using usearguments::Aim;

class Sandstorm : public CombatArt {
  public:
    Sandstorm(ugdk::graphic::Drawable* icon) : 
        CombatArt(icon, NULL, Constants::SANDSTORM_COST),
        emitter_(NULL),
        maintain_mana_cost_(Constants::SANDSTORM_MAINTAIN_COST) {}

    ~Sandstorm() {
        if(emitter_) emitter_->Die();
    }

    // inherited virtuals
    virtual void Use(component::Caster* caster) {
        if(!emitter_) {
            super::Use(caster);

            emitter_ = new SandstormEmitter(caster->aim(), &emitter_);
            caster->owner()->current_room()->AddObject(emitter_, caster->aim().origin_, map::POSITION_ABSOLUTE);

        } else {
            caster->mana() -= maintain_mana_cost_;
            emitter_->Renew();
        }
    }

    virtual bool Available(const component::Caster* caster) const {
        return emitter_
            ? caster->mana().Has(utils::Constants::SANDSTORM_MAINTAIN_COST)
            : caster->mana().Has(utils::Constants::SANDSTORM_COST)
        ;
    }
    
  protected:
    entities::SandstormEmitter* emitter_;
    double maintain_mana_cost_;

  private:
    typedef CombatArt super;
};

Skill* HeroSandstormBuild() {
    utils::HudImageFactory factory;
    return new Sandstorm(factory.SandstormIconImage());
}

} // namespace skills
