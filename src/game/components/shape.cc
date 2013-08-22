#include "game/components/shape.h"

#include <pyramidworks/collision/collisionobject.h>
#include "game/scenes/world.h"

namespace component {

Shape::Shape(pyramidworks::collision::CollisionObject* collision, pyramidworks::collision::CollisionObject* visibility)
  : visibility_(visibility)
{
    if(collision)
        collisions_.emplace_back(collision);
}

Shape::~Shape() {}
    
void Shape::Activate(scene::World* world) {
    for(auto& collision : collisions_)
        collision->StartColliding(world->collision_manager());
    if(visibility_) visibility_->StartColliding(world->visibility_manager());
}

void Shape::Deactivate() {
    for(auto& collision : collisions_)
        collision->StopColliding();
    if(visibility_) visibility_->StopColliding();
}
    
void Shape::ChangePosition(const ugdk::math::Vector2D& pos) {
    for(auto& collision : collisions_)
        collision->MoveTo(pos);
    if(visibility_) visibility_->MoveTo(pos);
}
    
void Shape::AddCollision(pyramidworks::collision::CollisionObject* collision) {
    collisions_.emplace_back(collision);
}

}  // namespace component
