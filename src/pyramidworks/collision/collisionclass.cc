#include "collisionclass.h"
#include "pyramidworks/collision/collisionobject.h"

namespace pyramidworks {
namespace collision {

const CollisionObjectList CollisionClass::FindCollidingObjects(CollisionObject *target) const {
    CollisionObjectList result;
    CollisionObjectList::const_iterator it;

    for(it = objects_.begin(); it != objects_.end(); ++it) {
        if((*it) == target) continue;
        if( (*it)->IsColliding(target) )
            result.push_front(*it);
    }
    return result;
}
    
void CollisionClass::AddObject(CollisionObject *obj) { 
    objects_.push_front(obj);
}

void CollisionClass::RemoveObject(CollisionObject *obj) { 
    objects_.remove(obj); 
}

} // namespace collision
} // namespace pyramidworks
