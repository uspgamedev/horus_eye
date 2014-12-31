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
    Condition(sprite::WObjRawPtr owner);
    ~Condition();

    static std::string DEFAULT_NAME() { return "condition"; }
    std::string component_name() const override { return DEFAULT_NAME(); }
    int order() const override { return orders::LOGIC; }
    
    void Update(double dt) override;

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
