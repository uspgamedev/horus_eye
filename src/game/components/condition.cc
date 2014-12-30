#include "game/components/condition.h"

#include "game/sprites/effect.h"

namespace component {

using std::list;
using std::shared_ptr;
using sprite::Effect;

Condition::Condition(sprite::WObjRawPtr owner)
    : owner_(owner) {}

Condition::~Condition() {

}

static bool deleteeffect(const shared_ptr<Effect>& effect) {
    return (effect->phase() == sprite::Effect::PHASE_FINISHED);
}

void Condition::Update(double dt) {
    for (list< shared_ptr<Effect> >::iterator i = effects_.begin(); i != effects_.end(); ++i)
        (*i)->Update(dt);
    effects_.remove_if(deleteeffect);
}

bool Condition::AddEffect(const std::shared_ptr<sprite::Effect>& new_effect) {
    effects_.push_front(new_effect);
    new_effect->StartEffect(owner_);
    return true;
}

int Condition::CountEffectsByName(const std::string& name) const {
    int result = 0;
    for (list< shared_ptr<Effect> >::const_iterator i = effects_.begin(); i != effects_.end(); ++i)
        result += static_cast<int>((*i)->name() == name);
    return result;
}

} // namespace component
