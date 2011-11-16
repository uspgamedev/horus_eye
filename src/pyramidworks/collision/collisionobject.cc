#include "collisionobject.h"

#include "pyramidworks/collision/collisionclass.h"
#include "pyramidworks/collision/collisionlogic.h"
#include "pyramidworks/geometry/geometricshape.h"

namespace pyramidworks {
namespace collision {

CollisionObject::CollisionObject(void *data) : data_(data), collision_class_(NULL), shape_(NULL) {
    if(collision_class_ != NULL)
        collision_class_->AddObject(this);
}

CollisionObject::~CollisionObject() {
    std::map<const CollisionClass*, CollisionLogic*>::iterator it;
    for(it = known_collisions_.begin(); it != known_collisions_.end(); ++it)
        delete it->second;
    known_collisions_.clear();

    if(shape_ != NULL)
        delete shape_;

    if(collision_class_ != NULL)
        collision_class_->RemoveObject(this);
}


void CollisionObject::SearchCollisions(std::list<CollisionInstance> &collision_list) {
    std::map<const CollisionClass*, CollisionLogic*>::iterator it;
    for(it = known_collisions_.begin(); it != known_collisions_.end(); ++it) {

        const CollisionObjectList target_list = it->first->FindCollidingObjects(this);
        CollisionObjectList::const_iterator obj;
        for(obj = target_list.begin(); obj != target_list.end(); ++obj)
            collision_list.push_front(CollisionInstance(it->second, (*obj)->data_));
    }
}

bool CollisionObject::IsColliding(const CollisionObject* obj) const {
    if(this->collision_class_ == NULL || obj->shape_ == NULL) return false;
    return this->shape_->Intersects(obj->shape_);
}

void CollisionObject::AddCollisionLogic(const CollisionClass* collision_class, CollisionLogic* logic) {
    CollisionLogic *query = known_collisions_[collision_class];
    if( query != NULL ) delete query;
    known_collisions_[collision_class] = logic;
}


void CollisionObject::set_collision_class(CollisionClass* collision_class) {
    if(collision_class_ != NULL) {
        fprintf(stderr, "HOLY CRAPZ\n");
        exit(1);
    }
    collision_class_ = collision_class;
    collision_class_->AddObject(this);
}

void CollisionObject::set_shape(geometry::GeometricShape* shape) { 
    if(shape_) delete shape_;
    shape_ = shape;
}

} // namespace collision
} // namespace pyramidworks
