#ifndef HORUSEYE_COMPONENT_CONDITION_H_
#define HORUSEYE_COMPONENT_CONDITION_H_

#include <list>
#include <memory>

#include "game/components/base.h"

#include "game/sprites.h"
#include "game/components/orders.h"

namespace component {
    
class Condition : public Base { 
  public:
    static const char* DEFAULT_NAME() { return "condition"; }
    static int DEFAULT_ORDER() { return orders::LOGIC; }

    Condition(sprite::WObjRawPtr owner);
    ~Condition();
    
    void Update(double dt);

    bool AddEffect(const std::shared_ptr<sprite::Effect>& new_effect);
    int CountEffectsByName(const std::string&) const;

    sprite::WObjRawPtr owner() { return owner_; }

  protected:
    /// The owner.
    sprite::WObjRawPtr owner_;

    // The conditions currently affecting this creature.
    std::list< std::shared_ptr<sprite::Effect> > effects_;

};  // class Walker

} // namespace component

#endif  // HORUSEYE_COMPONENT_CONDITION_H_
