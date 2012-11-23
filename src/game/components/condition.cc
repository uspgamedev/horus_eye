#include "game/components/condition.h"

#include "game/sprites/effect.h"

namespace component {

Condition::Condition(sprite::WorldObject* owner)
    : owner_(owner) {}

Condition::~Condition() {

}

static bool deleteeffect(sprite::Effect *effect) {
    bool is_finished = (effect->phase() == sprite::Effect::PHASE_FINISHED);
    if (is_finished) delete effect;
    return is_finished;
}

void Condition::Update(double dt) {
    for (std::list<sprite::Effect*>::iterator i = effects_.begin(); i != effects_.end(); ++i)
        (*i)->Update(dt);
    effects_.remove_if(deleteeffect);
}

bool Condition::AddEffect(sprite::Effect* new_effect) {
    effects_.push_front(new_effect);
    new_effect->StartEffect(owner_);
    return true;
}

int Condition::CountEffectsByName(const std::string& name) const {
    int result = 0;
    for (std::list<sprite::Effect*>::const_iterator i = effects_.begin(); i != effects_.end(); ++i)
        result += static_cast<int>((*i)->name() == name);
    return result;
}

} // namespace component
