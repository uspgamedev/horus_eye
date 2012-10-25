#ifndef HORUSEYE_COMPONENT_CONTROLLER_H_
#define HORUSEYE_COMPONENT_CONTROLLER_H_

#include "game/components/base.h"

#include <ugdk/math/vector2D.h>
#include "game/components/direction.h"
#include "game/sprites.h"
#include "game/components/orders.h"

namespace component {

class Controller : public Base { 
  public:
    enum SkillSlot {
        PRIMARY, SECONDARY, SPECIAL1, SPECIAL2, SPECIAL3, INVALID_SLOT
    };
    static const char* DEFAULT_NAME() { return "controller"; }
    static int DEFAULT_ORDER() { return orders::INPUT; }

    Controller() {}
    virtual ~Controller() {}

    virtual void Update(double dt) = 0;

    virtual bool IsUsingSkillSlot(SkillSlot) const = 0;

    virtual const ugdk::Vector2D& direction_vector() const = 0;
    virtual const Direction& direction() const { return dir_; }

    const ugdk::Vector2D& aim_destination() const { return aim_destination_; }

  protected:
    Direction dir_;

    ugdk::Vector2D aim_destination_;

};  // class Controller

}  // namespace component

#endif  // HORUSEYE_COMPONENT_CONTROLLER_H_
