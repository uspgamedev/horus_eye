#ifndef PYRAMIDWORKS_COLLISION_COLLISIONMASK_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONMASK_H_

#include <string>
#include <map>
#include <list>

#include "pyramidworks/collision/collisionmanager.h"

namespace pyramidworks {
namespace collision {

class CollisionObject;

typedef std::list<const CollisionObject *> CollisionObjectList;

class CollisionMask {
  public:
    ~CollisionMask() {}
    const CollisionMask* parent() const { return parent_; }
	void set_parent(CollisionMask* parent) { parent_ = parent; }

    const CollisionObjectList FindCollidingObjects(CollisionObject *target) const;

    void AddObject(CollisionObject *obj);
    void RemoveObject(CollisionObject *obj);

  private:
	friend class CollisionManager;
    CollisionMask(std::string name, const CollisionMask* parent = NULL) : name_(name), parent_(parent) {}

	// Unnecessary, used for debugging purposes.
	std::string name_;

    const CollisionMask* parent_;
    CollisionObjectList objects_;
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONMASK_H_ */
