#include "game/components/condition.h"

#include "game/sprites/condition.h"

namespace component {

Condition::Condition(sprite::WorldObject* owner)
    : owner_(owner) {}

Condition::~Condition() {

}

static bool deletecondition(sprite::Condition *condition) {
    bool is_finished = (condition->phase() == sprite::Condition::PHASE_FINISHED);
    if (is_finished) delete condition;
    return is_finished;
}

void Condition::Update(double dt) {
    for (std::list<sprite::Condition*>::iterator i = conditions_.begin(); i != conditions_.end(); ++i)
        (*i)->Update(dt);
    conditions_.remove_if(deletecondition);
}

bool Condition::AddCondition(sprite::Condition* new_condition) {
    conditions_.push_front(new_condition);
    new_condition->StartCondition(owner_);
    return true;
}

} // namespace component
