#ifndef HORUSEYE_COMPONENT_WALKER_H_
#define HORUSEYE_COMPONENT_WALKER_H_

#include "game/components/base.h"

#include "game/sprites.h"

#include "game/components/direction.h"
#include "game/components/orders.h"

#include <ugdk/system/compatibility.h>
#include <ugdk/math/vector2D.h>
#include <ugdk/util/uncopyable.h>
#include <pyramidworks/collision.h>

namespace component {
    
class Walker : public Base, public ugdk::util::Uncopyable { 
  public:
    Walker(double original_speed);
    ~Walker();

    static std::string DEFAULT_NAME() { return "walker"; }
    std::string component_name() const override { return DEFAULT_NAME(); }
    int order() const override { return orders::LOGIC; }
    
    void Update(double dt) override;
    void OnAdd(sprite::WObjRawPtr) override;

    pyramidworks::collision::CollisionLogic CreateRectCollision();

    sprite::WObjRawPtr owner() { return owner_; }
    double original_speed() const { return original_speed_; }
    double current_speed() const { return current_speed_; }
    void set_current_speed(double speed) { current_speed_ = speed; }

    const ugdk::math::Vector2D& offset_direction() const { return offset_direction_; }
    void set_offset_direction(const ugdk::math::Vector2D& offset_dir) { offset_direction_ = offset_dir; }

  protected:
    void move(ugdk::math::Vector2D direction, double delta_t);
    void collideWithRect(const pyramidworks::collision::CollisionObject*);

    /// The owner.
    sprite::WObjRawPtr owner_;
    
    /// The last position this creature was that is guaranteed to not colide with any walls.
    ugdk::math::Vector2D last_stable_position_;

    double last_dt_;

    /// How fast this creature moves per second.
    double current_speed_;

    /// Stores the original speed, so one can alter the speed temporarily.
    double original_speed_;

    /// The direction this creature is moving to.
    ugdk::math::Vector2D walking_direction_;

    /// 
    ugdk::math::Vector2D offset_direction_;
    
    friend class RectCollision;

};  // class Walker

} // namespace component

#endif  // HORUSEYE_COMPONENT_WALKER_H_
