#ifndef HORUSEYE_COMPONENT_SHAPE_H_
#define HORUSEYE_COMPONENT_SHAPE_H_

#include "game/components/base.h"

#include <ugdk/math/vector2D.h>
#include <pyramidworks/collision.h>

#include "game/sprites.h"
#include "game/components/orders.h"

namespace component {

class Shape : public Base {
  public:
    static const char* DEFAULT_NAME() { return "shape"; }
    static int DEFAULT_ORDER() { return orders::LOGIC; }

    Shape(pyramidworks::collision::CollisionObject* collision, pyramidworks::collision::CollisionObject* visibility);
    ~Shape();

    void Update(double dt) {}
    
    void Activate();
    void Deactivate();
    void ChangePosition(const ugdk::math::Vector2D& pos);
    
          pyramidworks::collision::CollisionObject* collision()       { return collision_; }
    const pyramidworks::collision::CollisionObject* collision() const { return collision_; }
    
          pyramidworks::collision::CollisionObject* visibility()       { return visibility_; }
    const pyramidworks::collision::CollisionObject* visibility() const { return visibility_; }

  private:
    pyramidworks::collision::CollisionObject *collision_;
    
    pyramidworks::collision::CollisionObject *visibility_;

};  // class Shape

}  // namespace component

#endif  // HORUSEYE_COMPONENT_SHAPE_H_
