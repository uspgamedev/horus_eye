#ifndef HORUSEYE_COMPONENT_MUMMYCONTROLLER_H_
#define HORUSEYE_COMPONENT_MUMMYCONTROLLER_H_

#include <queue>
#include <ugdk/time.h>
#include "game/components/controller.h"

namespace component {

class MummyController : public Controller { 
  typedef Controller super;
  public:
    MummyController(sprite::WorldObject* owner, double start_time_to_think);
    ~MummyController();

    void Update(double dt);

    bool IsUsingSkillSlot(SkillSlot) const;

    const ugdk::Vector2D& direction_vector() const { return current_direction_; }
    void set_direction_vector(const ugdk::Vector2D& dir) { current_direction_ = dir; }

    void set_standing(bool standing) { standing_ = standing; }

  protected:
    virtual void Think(double dt);
    void randomMovement();
    void updateDirection(const ugdk::Vector2D&);

    bool standing_;
    double time_to_think_, start_time_to_think_;
    std::queue<ugdk::Vector2D> path_;
    ugdk::time::TimeAccumulator *interval_;

    ugdk::Vector2D current_direction_;

};  // class MummyController

}  // namespace component

#endif  // HORUSEYE_COMPONENT_MUMMYCONTROLLER_H_
