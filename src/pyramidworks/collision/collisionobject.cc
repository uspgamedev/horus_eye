#include "collisionobject.h"

#include "pyramidworks/collision/collisionmask.h"
#include "pyramidworks/collision/collisionlogic.h"
#include "pyramidworks/geometry/geometricobject.h"

namespace pyramidworks {
namespace collision {

bool CollisionObject::CollidesWith(CollisionObject* obj, const CollisionMask* mask) {
    CollisionLogic *col = known_collisions_[mask];
    if(col != NULL) col->Handle(obj->data_);
    else if(mask->parent() != NULL) 
        CollidesWith(obj, mask->parent());
}

bool CollisionObject::IsColliding(const CollisionObject* obj) const {
    return this->geom_->Intersects(obj->geom_);
}

void CollisionObject::AddCollisionGeom(CollisionMask* mask, geometry::GeometricObject* geom) {
    geom->set_position(&this->position_);
    this->geom_ = geom;

    SetMask(mask);
    //this->collision_types_.push_back(CollisionGeom(mask, geom));
}

void CollisionObject::SetMask(CollisionMask* mask) {
    if(mask_ != NULL)
        mask_->RemoveObject(this);

    this->mask_ = mask;
    mask->AddObject(this);
}


} // namespace collision
} // namespace pyramidworks
