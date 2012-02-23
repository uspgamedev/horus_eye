#include "collisionclass.h"

#include <vector>

#include "pyramidworks/collision/collisionobject.h"
#include "pyramidworks/geometry/geometricshape.h"
#include <ugdk/util/intervalkdtree.h>

namespace pyramidworks {
namespace collision {

const CollisionObjectList CollisionClass::FindCollidingObjects(
                                         const CollisionObject *target) const {
    std::vector<const CollisionObject *> *filtered_results 
        = objects_tree_.getIntersectingItems(target->GetBoundingBox());
    CollisionObjectList result;

    for(std::vector<const CollisionObject *>::const_iterator it
           = filtered_results->begin(); it != filtered_results->end(); ++it) {
        if((*it) == target) continue;
        if( (*it)->IsColliding(target) )
            result.push_front(*it);
    }
    /*
#ifdef DEBUG
    cout << "Max depth: " << objects_tree_.max_height_ << endl;
    cout << "Filtered " << filtered_results->size() << " of " << objects_.size() << endl;
#endif
     */
    delete filtered_results;
    return result;
}
    
void CollisionClass::AddObject(const CollisionObject *obj) {
    objects_.insert(obj);
    objects_tree_.Insert(obj->GetBoundingBox(), obj);
}

void CollisionClass::RemoveObject(const CollisionObject *obj) { 
    objects_.erase(obj);
    objects_tree_.Remove(obj);
}

void CollisionClass::RefreshObject(const CollisionObject *obj) {
    objects_tree_.Update(obj->GetBoundingBox(), obj);
}
    
void CollisionClass::Refresh() {
    objects_tree_.Clear();
    std::set<const CollisionObject *>::iterator it;
    for (it = objects_.begin(); it != objects_.end(); ++it) {
        objects_tree_.Insert((*it)->GetBoundingBox(), *it);
    }
}

} // namespace collision
} // namespace pyramidworks
