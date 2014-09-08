#include "game/components/body.h"

#include <pyramidworks/collision/collisionobject.h>
#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"

namespace component {

using pyramidworks::collision::CollisionObject;

Body::Body(CollisionObject* collision, CollisionObject* visibility)
    :   owner_(nullptr)
    ,   visibility_(visibility)
{
    if(collision)
        collisions_.emplace_back(collision);
}

Body::~Body() {}
    
void Body::OnAdd(sprite::WorldObject* owner) {
    owner_ = owner;
    for (const auto& col : collisions_)
        col->set_data(owner_);
    if (visibility_)
        visibility_->set_data(owner_);
}
    
void Body::Activate(scene::World* world) {
    for(auto& collision : collisions_)
        collision->StartColliding(world->collision_manager());
    if(visibility_) visibility_->StartColliding(world->visibility_manager());
}

void Body::Deactivate() {
    for(auto& collision : collisions_)
        collision->StopColliding();
    if(visibility_) visibility_->StopColliding();
}
    
void Body::ChangePosition(const ugdk::math::Vector2D& pos) {
    for(auto& collision : collisions_)
        collision->MoveTo(pos);
    if(visibility_) visibility_->MoveTo(pos);
}
    
void Body::AddCollision(pyramidworks::collision::CollisionObject* collision) {
    collision->set_data(owner_);
    collisions_.emplace_back(collision);
}

}  // namespace component
