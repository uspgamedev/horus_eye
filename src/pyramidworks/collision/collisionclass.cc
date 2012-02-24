#include "collisionclass.h"

#include <vector>

#include "game/scenes/world.h"
#include "pyramidworks/collision/collisionobject.h"
#include "pyramidworks/geometry/geometricshape.h"
#include <ugdk/util/intervalkdtree.h>

namespace pyramidworks {
namespace collision {

CollisionClass::~CollisionClass() {delete objects_tree_;}

CollisionClass::CollisionClass() : parent_(NULL) {
    scene::World *world = WORLD();
    double min_coordinates[2] = {-1, -1};
    double max_coordinates[2];
    if (world) {
        max_coordinates[0] = world->level_width() + 1;
        max_coordinates[1] = world->level_height() + 1;
    } else {
        max_coordinates[0] = 1000;
        max_coordinates[1] = 1000;
    }
    ugdk::ikdtree::Box<2> tree_bounding_box(min_coordinates,
                                            max_coordinates);
    objects_tree_ = 
        new ugdk::ikdtree::IntervalKDTree<const CollisionObject *, 2>(
                                                          tree_bounding_box,5);
}

const CollisionObjectList CollisionClass::FindCollidingObjects(
                                         const CollisionObject *target) const {
    std::vector<const CollisionObject *> *filtered_results 
        = objects_tree_->getIntersectingItems(target->GetBoundingBox());
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
    objects_tree_->Insert(obj->GetBoundingBox(), obj);
}

void CollisionClass::RemoveObject(const CollisionObject *obj) { 
    objects_tree_->Remove(obj);
}

void CollisionClass::RefreshObject(const CollisionObject *obj) {
    objects_tree_->Update(obj->GetBoundingBox(), obj);
}

} // namespace collision
} // namespace pyramidworks
