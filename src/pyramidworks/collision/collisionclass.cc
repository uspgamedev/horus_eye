#include "collisionclass.h"
#include "pyramidworks/collision/collisionobject.h"

namespace pyramidworks {
namespace collision {

const CollisionObjectList CollisionClass::FindCollidingObjects(const CollisionObject *target) const {
    CollisionObjectList result;
    std::set<const CollisionObject *>::const_iterator it;

    for(it = objects_.begin(); it != objects_.end(); ++it) {
        if((*it) == target) continue;
        if( (*it)->IsColliding(target) )
            result.push_front(*it);
    }
    return result;
}
    
void CollisionClass::AddObject(const CollisionObject *obj) { 
    objects_.insert(obj);
}

void CollisionClass::RemoveObject(const CollisionObject *obj) { 
    objects_.erase(obj); 
}

} // namespace collision
} // namespace pyramidworks
