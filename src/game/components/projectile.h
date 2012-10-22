#ifndef HORUSEYE_COMPONENT_PROJECTILE_H_
#define HORUSEYE_COMPONENT_PROJECTILE_H_

#include "game/components/base.h"

#include <ugdk/math/vector2D.h>
#include "game/sprites/worldobject.h"

#include "game/components/orders.h"

namespace component {

class Projectile : public Base {
  public:
    static const char* DEFAULT_NAME() { return "projectile"; }
    static int DEFAULT_ORDER() { return orders::LOGIC; }

    Projectile(sprite::WorldObject* owner, double speed, const ugdk::Vector2D &dir)
    :   owner_(owner),
        speed_(speed),
        direction_(dir.Normalize()) {}

    void Move(double delta_t) {
        ugdk::Vector2D velocity = direction_ * (speed_ * delta_t);
        owner_->set_world_position(owner_->world_position() + velocity);
    }

    void Update(double delta_t) {
        Move(delta_t);
    }

    void set_direction(ugdk::Vector2D direction) { direction_ = direction; }
    ugdk::Vector2D get_direction() { return direction_; }

  protected:
    sprite::WorldObject* owner_;
    double speed_;
    ugdk::Vector2D direction_;
};

}

#endif /* HORUSEYE_COMPONENT_PROJECTILE_H_ */
