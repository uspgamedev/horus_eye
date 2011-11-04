#include "collisionobject.h"

#include "pyramidworks/collision/collisionmask.h"
#include "pyramidworks/collision/collisionlogic.h"
#include "pyramidworks/geometry/geometricshape.h"

namespace pyramidworks {
namespace collision {

CollisionObject::CollisionObject(void *data) : data_(data) {
    CollisionMask* mask = NULL;
    geometry::GeometricShape* geomobj = NULL;
    geom_ = CollisionGeom(mask, geomobj);
}

CollisionObject::~CollisionObject() {
    std::map<const CollisionMask*, CollisionLogic*>::iterator it;
    for(it = known_collisions_.begin(); it != known_collisions_.end(); ++it)
        delete it->second;
    known_collisions_.clear();

    if(geom_.second != NULL)
        delete geom_.second;

    if(geom_.first != NULL)
        geom_.first->RemoveObject(this);
}


void CollisionObject::SearchCollisions(std::list<CollisionInstance> &collision_list) {
    std::map<const CollisionMask*, CollisionLogic*>::iterator it;
    for(it = known_collisions_.begin(); it != known_collisions_.end(); ++it) {

        const CollisionObjectList target_list = it->first->FindCollidingObjects(this);
        CollisionObjectList::const_iterator obj;
        for(obj = target_list.begin(); obj != target_list.end(); ++obj)
            collision_list.push_front(CollisionInstance(it->second, (*obj)->data_));
    }
}

bool CollisionObject::IsColliding(const CollisionObject* obj) const {
    if(this->geom_.second == NULL || obj->geom_.second == NULL) return false;
    return this->geom_.second->Intersects(obj->geom_.second);
}

void CollisionObject::AddCollisionLogic(const CollisionMask* mask, CollisionLogic* logic) {
    CollisionLogic *query = known_collisions_[mask];
    if( query != NULL ) delete query;
    known_collisions_[mask] = logic;
}

void CollisionObject::AddCollisionGeom(CollisionMask* mask, geometry::GeometricShape* geom) {
    geom->set_position(&this->position_);
    this->geom_.second = geom;

    SetMask(mask);
    //this->collision_types_.push_back(CollisionGeom(mask, geom));
}

void CollisionObject::SetMask(CollisionMask* mask) {
    if(geom_.first != NULL)
        geom_.first->RemoveObject(this);

    geom_.first = mask;
    mask->AddObject(this);
}


} // namespace collision
} // namespace pyramidworks
