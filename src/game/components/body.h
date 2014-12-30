#ifndef HORUSEYE_COMPONENT_BODY_H_
#define HORUSEYE_COMPONENT_BODY_H_

#include "game/components/base.h"

#include "game/sprites.h"
#include "game/scenes.h"
#include "game/components/orders.h"

#include <ugdk/system/compatibility.h>
#include <ugdk/math/vector2D.h>
#include <pyramidworks/collision.h>

#include <list>
#include <memory>

namespace component {

class Body : public Base {
  public:
    static const char* DEFAULT_NAME() { return "body"; }
    static int DEFAULT_ORDER() { return orders::LOGIC; }

    Body(pyramidworks::collision::CollisionObject* collision, pyramidworks::collision::CollisionObject* visibility);
    ~Body();

    void Update(double) override {}
    void OnAdd(sprite::WObjRawPtr) override;
    
    void Activate(scene::World*);
    void Deactivate();
    void ChangePosition(const ugdk::math::Vector2D& pos);
    void AddCollision(pyramidworks::collision::CollisionObject* collision);

    pyramidworks::collision::CollisionObject* main_collision () const { return collisions_.front().get(); }

  private:
    sprite::WObjRawPtr owner_;

    std::list<std::unique_ptr<pyramidworks::collision::CollisionObject> > collisions_;
    
    std::unique_ptr<pyramidworks::collision::CollisionObject> visibility_;

};  // class Shape

}  // namespace component

#endif  // HORUSEYE_COMPONENT_BODY_H_
