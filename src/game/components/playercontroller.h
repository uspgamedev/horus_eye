#ifndef HORUSEYE_COMPONENT_PLAYERCONTROLLER_H_
#define HORUSEYE_COMPONENT_PLAYERCONTROLLER_H_

#include <list>
#include "game/components/controller.h"

namespace component {

class PlayerController : public Controller { 
  typedef Controller super;
  public:
    PlayerController(sprite::WorldObject* owner); 
    ~PlayerController() {}

    void Update(double dt);

    void AddSkill(int id);
    void RemoveSkill(int id);

    bool IsUsingSkillSlot(SkillSlot) const;
    const ugdk::math::Vector2D& direction_vector() const { return current_direction_; }

  protected:
    /// The owner.
    sprite::WorldObject* owner_;

    ugdk::math::Vector2D current_direction_;
    ugdk::math::Vector2D mouse_aim_offset_;
    std::list<int> known_skills_;
    std::list<int>::const_iterator selected_skill_;

};  // class PlayerController

}  // namespace component

#endif  // HORUSEYE_COMPONENT_PLAYERCONTROLLER_H_
