#ifndef HORUSEYE_COMPONENT_MUMMYCONTROLLER_H_
#define HORUSEYE_COMPONENT_MUMMYCONTROLLER_H_

#include <queue>
#include <ugdk/time.h>
#include "game/components/controller.h"

namespace component {

class MummyController : public Controller { 
  typedef Controller super;
  public:
    MummyController(sprite::WorldObject* owner);
    virtual ~MummyController();

    virtual void Update(double dt);

    virtual const ugdk::Vector2D& direction_vector() const { return current_direction_; }

  protected:
    virtual void Think(double dt);
    void RandomMovement();

    double time_to_think_;
    std::queue<ugdk::Vector2D> path_;
    ugdk::time::TimeAccumulator *interval_;

    ugdk::Vector2D current_direction_;

};  // class MummyController

}  // namespace component

#endif  // HORUSEYE_COMPONENT_MUMMYCONTROLLER_H_
