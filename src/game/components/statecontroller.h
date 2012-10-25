#ifndef HORUSEYE_COMPONENT_STATECONTROLLER_H_
#define HORUSEYE_COMPONENT_STATECONTROLLER_H_

#include "game/components/controller.h"

#include <ugdk/math/vector2D.h>

namespace component {

class StateController : public Controller {
  public:
    StateController() {}
    StateController(const Direction& dir, const ugdk::Vector2D& d) : direction_(d) { dir_ = dir; }
    ~StateController() {}

    void Update(double dt) {}

    bool IsUsingSkillSlot(SkillSlot) const { return false; }

    const ugdk::Vector2D& direction_vector() const { return direction_; }

    void set_direction_vector(const ugdk::Vector2D& d) { direction_ = d; }
    void set_direction(const Direction& dir) { dir_ = dir; }

  protected:
    ugdk::Vector2D direction_;
};

}

#endif /* HORUSEYE_COMPONENT_PROJECTILE_H_ */
