#ifndef PYRAMIDWORKS_COLLISION_COLLISIONOBJECT_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONOBJECT_H_

#include <map>
#include <vector>
#include <list>
#include <ugdk/math/vector2D.h>

namespace pyramidworks {

namespace geometry {
class GeometricShape;
}

namespace collision {

class CollisionClass;
class CollisionLogic;

typedef std::pair<CollisionLogic*, void* > CollisionInstance;

class CollisionObject {
  public:
    CollisionObject(void *data = NULL);
    ~CollisionObject();

    void SearchCollisions(std::list<CollisionInstance> &collision_list);
    bool IsColliding(const CollisionObject*) const;

    void AddCollisionLogic(const CollisionClass* colclass, CollisionLogic* logic);


    void set_collision_class(CollisionClass* collision_class);

    const geometry::GeometricShape* shape() const { return shape_; };
    void set_shape(geometry::GeometricShape* shape);

  private:
    // Data that is sent to CollisionLogic::Handle
    void *data_;

    CollisionClass* collision_class_;
    geometry::GeometricShape* shape_;

    std::map<const CollisionClass*, CollisionLogic*> known_collisions_;
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONOBJECT_H_ */
