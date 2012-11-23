#ifndef HORUSEYE_COMPONENT_CONDITION_H_
#define HORUSEYE_COMPONENT_CONDITION_H_

#include <list>

#include "game/components/base.h"

#include "game/sprites.h"
#include "game/components/orders.h"

namespace component {
    
class Condition : public Base { 
  public:
    static const char* DEFAULT_NAME() { return "condition"; }
    static int DEFAULT_ORDER() { return orders::LOGIC; }

    Condition(sprite::WorldObject* owner);
    ~Condition();
    
    void Update(double dt);

    bool AddCondition(sprite::Condition* new_condition);

    sprite::WorldObject* owner() { return owner_; }

  protected:
    void updateCondition(double dt);

    /// The owner.
    sprite::WorldObject* owner_;

    // The conditions currently affecting this creature.
    std::list<sprite::Condition*> conditions_;

};  // class Walker

} // namespace component

#endif  // HORUSEYE_COMPONENT_CONDITION_H_
