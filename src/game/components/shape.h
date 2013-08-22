#ifndef HORUSEYE_COMPONENT_SHAPE_H_
#define HORUSEYE_COMPONENT_SHAPE_H_

#include <list>
#include <memory>
#include "game/components/base.h"

#include <ugdk/math/vector2D.h>
#include <pyramidworks/collision.h>

#include "game/sprites.h"
#include "game/scenes.h"
#include "game/components/orders.h"

namespace component {

class Shape : public Base {
  public:
    static const char* DEFAULT_NAME() { return "shape"; }
    static int DEFAULT_ORDER() { return orders::LOGIC; }

    Shape(pyramidworks::collision::CollisionObject* collision, pyramidworks::collision::CollisionObject* visibility);
    ~Shape();

    void Update(double dt) {}
    
    void Activate(scene::World*);
    void Deactivate();
    void ChangePosition(const ugdk::math::Vector2D& pos);
    void AddCollision(pyramidworks::collision::CollisionObject* collision);

  private:
    std::list<std::unique_ptr<pyramidworks::collision::CollisionObject> > collisions_;
    
    std::unique_ptr<pyramidworks::collision::CollisionObject> visibility_;

};  // class Shape

}  // namespace component

#endif  // HORUSEYE_COMPONENT_SHAPE_H_
