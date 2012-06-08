#ifndef HORUSEYE_COMPONENT_PLAYERCONTROLLER_H_
#define HORUSEYE_COMPONENT_PLAYERCONTROLLER_H_

#include "game/components/controller.h"

namespace component {

class PlayerController : public Controller { 
  typedef Controller super;
  public:
    PlayerController(sprite::WorldObject* owner) : super(owner), skill_selected_(0) {}
    virtual ~PlayerController() {}

    virtual void Update(double dt);

    virtual bool IsUsingSkillSlot(SkillSlot) const;
    virtual const ugdk::Vector2D& direction_vector() const { return current_direction_; }

  protected:
    ugdk::Vector2D current_direction_;
    int skill_selected_;

};  // class PlayerController

}  // namespace component

#endif  // HORUSEYE_COMPONENT_PLAYERCONTROLLER_H_
