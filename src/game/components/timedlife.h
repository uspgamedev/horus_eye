#ifndef HORUSEYE_COMPONENT_TIMEDLIFE_H_
#define HORUSEYE_COMPONENT_TIMEDLIFE_H_

#include "game/components/base.h"
#include "game/components/orders.h"

namespace component {
    
class TimedLife : public Base { 
  public:
    TimedLife(double duration);
    ~TimedLife();

    static std::string DEFAULT_NAME() { return "timedlife"; }
    std::string component_name() const override { return DEFAULT_NAME(); }
    int order() const override { return orders::LOGIC; }
    
    void Update(double dt) override;
    void OnAdd(sprite::WObjRawPtr) override;

    sprite::WObjRawPtr owner() { return owner_; }

  protected:
    /// The owner.
    sprite::WObjRawPtr owner_;

    /// How long this component has gone by.
    double time_elapsed_;

    /// How long before killing the owner.
    double duration_;

};  // class Walker

} // namespace component

#endif  // HORUSEYE_COMPONENT_TIMEDLIFE_H_
