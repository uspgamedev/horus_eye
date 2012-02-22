
#include <cstdlib>
#include <cstdio>

#include "collisionobject.h"

#include "pyramidworks/collision/collisionmanager.h"
#include "pyramidworks/collision/collisionclass.h"
#include "pyramidworks/collision/collisionlogic.h"
#include "pyramidworks/geometry/geometricshape.h"

namespace pyramidworks {
namespace collision {

CollisionObject::CollisionObject(void *data) 
    :   data_(data),
        collision_class_(NULL), 
        shape_(NULL) {}

CollisionObject::~CollisionObject() {
    std::map<const CollisionClass*, CollisionLogic*>::iterator it;
    for(it = known_collisions_.begin(); it != known_collisions_.end(); ++it)
        delete it->second;
    known_collisions_.clear();

    if(shape_ != NULL)
        delete shape_;

    if(collision_class_ != NULL)
        StopColliding();
}


void CollisionObject::SearchCollisions(std::list<CollisionInstance> &collision_list) {
    std::map<const CollisionClass*, CollisionLogic*>::iterator it;
    for(it = known_collisions_.begin(); it != known_collisions_.end(); ++it) {

        const CollisionObjectList& target_list = it->first->FindCollidingObjects(this);
        CollisionObjectList::const_iterator obj;
        for(obj = target_list.begin(); obj != target_list.end(); ++obj)
            collision_list.push_front(CollisionInstance(it->second, (*obj)->data_));
    }
}

bool CollisionObject::IsColliding(const CollisionObject* obj) const {
    if(this->collision_class_ == NULL || obj->shape_ == NULL) return false;
    return this->shape_->Intersects(absolute_position(), obj->shape_, obj->absolute_position());
}

void CollisionObject::AddCollisionLogic(const std::string& colclass, CollisionLogic* logic) {
    AddCollisionLogic(CollisionManager::reference()->Get(colclass), logic);
}

void CollisionObject::AddCollisionLogic(const CollisionClass* collision_class, CollisionLogic* logic) {
    CollisionLogic *query = known_collisions_[collision_class];
    if( query != NULL ) delete query;
    known_collisions_[collision_class] = logic;
}

void CollisionObject::InitializeCollisionClass(const std::string& colclass) {
    InitializeCollisionClass(CollisionManager::reference()->Get(colclass));
}

void CollisionObject::InitializeCollisionClass(CollisionClass* collision_class) {
#ifdef DEBUG
    if(collision_class_ != NULL) fprintf(stderr, "Fatal Error: Changing the collision_class of a CollisionObject.\n");
    if(collision_class == NULL) fprintf(stderr, "Warning: Initializing the collision_class with NULL.\n");
#endif
    collision_class_ = collision_class;
}

void CollisionObject::StartColliding() {
#ifdef DEBUG
    if(collision_class_ == NULL) fprintf(stderr, "Warning: CollisionObject::StartColliding called with an object with NULL collision_class.\n");
#endif
    collision_class_->AddObject(this);
}

void CollisionObject::StopColliding() {
#ifdef DEBUG
    if(collision_class_ == NULL) fprintf(stderr, "Warning: CollisionObject::StopColliding called with an object with NULL collision_class.\n");
#endif
    collision_class_->RemoveObject(this);
}

void CollisionObject::set_shape(geometry::GeometricShape* shape) { 
    if(shape_) delete shape_;
    shape_ = shape;
}

void CollisionObject::MoveTo(const ugdk::Vector2D& position) {
    position_ = position;
    // TODO inform the CollisionClass
}

} // namespace collision
} // namespace pyramidworks
