#ifndef PYRAMIDWORKS_COLLISION_COLLISIONMASK_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONMASK_H_

#include <string>
#include <list>
#include <set>
#include <ugdk/util/intervalkdtree.h>

namespace pyramidworks {
namespace collision {

class CollisionObject;

typedef std::list<const CollisionObject *> CollisionObjectList;

/// \class CollisionClass collisionclass.h "pyramidworks/collision/collisionclass.h"
class CollisionClass {
  public:
    ~CollisionClass() {}
    const CollisionClass* parent() const { return parent_; }
    void set_parent(CollisionClass* parent) { parent_ = parent; }

    const CollisionObjectList FindCollidingObjects(const CollisionObject *target) const;

    void AddObject(const CollisionObject *obj);
    void RemoveObject(const CollisionObject *obj);

#ifdef DEBUG
    void set_name(const std::string &name) { name_ = name; }
#endif
  private:
    friend class CollisionManager;
    CollisionClass() : parent_(NULL), objects_tree_(5) {}
    void Update ();

    const CollisionClass* parent_;
    std::set<const CollisionObject *> objects_;
    ugdk::ikdtree::IntervalKDTree<const CollisionObject *, 2> objects_tree_;

#ifdef DEBUG
    // Unnecessary, used for debugging purposes.
    std::string name_;
#endif
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONMASK_H_ */
