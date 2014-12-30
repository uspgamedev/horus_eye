#ifndef HORUSEYE_COMPONENT_TIMEDLIFE_H_
#define HORUSEYE_COMPONENT_TIMEDLIFE_H_

#include "game/components/base.h"
#include "game/components/orders.h"

namespace component {
    
class TimedLife : public Base { 
  public:
    static const char* DEFAULT_NAME() { return "timedlife"; }
    static int DEFAULT_ORDER() { return orders::LOGIC; }

    TimedLife(double duration);
    ~TimedLife();
    
    void Update(double dt) override;
    void OnAdd(sprite::WorldObject*) override;

    sprite::WorldObject* owner() { return owner_; }

  protected:
    /// The owner.
    sprite::WorldObject* owner_;

    /// How long this component has gone by.
    double time_elapsed_;

    /// How long before killing the owner.
    double duration_;

};  // class Walker

} // namespace component

#endif  // HORUSEYE_COMPONENT_TIMEDLIFE_H_
