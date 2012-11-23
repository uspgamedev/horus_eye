#include "game/builders/effectbuilder.h"

#include <string>
#include <ugdk/time/timeaccumulator.h>

#include "game/constants.h"
#include "game/sprites/worldobject.h"
#include "game/components/graphic.h"
#include "game/sprites/effect.h"

#define SECONDS_TO_MILISECONDS(sec) (int)((sec) * 1000)

namespace builder {
namespace EffectBuilder {

using sprite::WorldObject;

class IncreaseSightEffect : public sprite::Effect {
  public:
    IncreaseSightEffect(double time_condition)
        :   condition_duration_(new ugdk::time::TimeAccumulator(SECONDS_TO_MILISECONDS(time_condition))) {}
    ~IncreaseSightEffect() { delete condition_duration_; }
    
    const std::string& name() const {
        static std::string sight_name("increase_sight");
        return sight_name;
    }

    void Update(double dt) {
        if(condition_duration_->Expired())
            EndEffect();
    }

    void onStart() {
        owner()->graphic()->ChangeLightRadius(owner()->graphic()->light_radius() + constants::GetDouble("SIGHT_POTION_INCREASE"));
    }

    void onEnd() {
        owner()->graphic()->ChangeLightRadius(owner()->graphic()->light_radius() - constants::GetDouble("SIGHT_POTION_INCREASE"));
    }
        
  private:
    ugdk::time::TimeAccumulator *condition_duration_;
};

std::tr1::shared_ptr<sprite::Effect> increase_sight() {
    return std::tr1::shared_ptr<sprite::Effect>(new IncreaseSightEffect(constants::GetDouble("CONDITION_DURATION")));
}

}
}
