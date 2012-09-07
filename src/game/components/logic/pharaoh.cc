
#include <ugdk/math/vector2D.h>

#include "pharaoh.h"

#include "game/utils/visionstrategy.h"
#include "game/utils/imagefactory.h"
#include "game/utils/constants.h"
#include "game/skills/combatart.h"

using namespace std;
using namespace ugdk;
using namespace utils;
using resource::Energy;

namespace component {

#define SQRT_3 1.7320508075688772935274463415059
#define PHARAOH_TIME_TO_THINK 0.05

Pharaoh::Pharaoh(sprite::WorldObject* owner) : Mummy(owner) {
    time_to_think_ = PHARAOH_TIME_TO_THINK;
}

Pharaoh::~Pharaoh() {}

void Pharaoh::Think(double dt) {
    Creature::Update(dt);
    time_to_think_ -= dt;
    if(time_to_think_ <= 0) {
        time_to_think_ = PHARAOH_TIME_TO_THINK;
        speed_ = original_speed_;
        VisionStrategy strategy;
        if(strategy.IsVisible(owner_->world_position())) {
            standing_ = false;

            path_ = strategy.Calculate(owner_->world_position());
            UpdateDirection(path_.front());

            Caster* caster = owner()->caster();
            Controller* controller = owner_->controller();
            for(Controller::SkillSlot slot = Controller::PRIMARY; slot < Controller::INVALID_SLOT; slot = Controller::SkillSlot(slot + 1)) {
                skills::Skill* skill = caster->SkillAt(slot);
                if(!skill) continue;
                if(controller->IsUsingSkillSlot(slot) && skill->Available()) {
                    if(skill->IsValidUse()) {
                        skill->Use();
                        StartAttack(NULL);
                    }
                    break;
                }
            }
        } else if(!standing_) {
            RandomMovement();
            last_standing_direction_ = animation_direction_;
        }
    }
}

}
