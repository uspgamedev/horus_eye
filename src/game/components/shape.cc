#include "game/components/shape.h"

#include <pyramidworks/collision/collisionobject.h>
#include "game/scenes/world.h"

namespace component {

Shape::Shape(pyramidworks::collision::CollisionObject* collision, pyramidworks::collision::CollisionObject* visibility)
  : collision_(collision),
    visibility_(visibility) {}

Shape::~Shape() {
    delete collision_;
    delete visibility_;
}
    
void Shape::Activate(scene::World* world) {
    if(collision_) collision_->StartColliding(world->collision_manager());
    if(visibility_) visibility_->StartColliding(world->visibility_manager());
}

void Shape::Deactivate() {
   if(collision_) collision_->StopColliding();
   if(visibility_) visibility_->StopColliding();

}
    
void Shape::ChangePosition(const ugdk::math::Vector2D& pos) {
    if(collision_) collision_->MoveTo(pos);
    if(visibility_) visibility_->MoveTo(pos);
}

}  // namespace component
