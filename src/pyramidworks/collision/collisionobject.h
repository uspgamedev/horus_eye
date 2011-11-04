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

class CollisionMask;
class CollisionLogic;

typedef std::pair<CollisionMask*, const geometry::GeometricShape*> CollisionGeom;
typedef std::pair<CollisionLogic*, void* > CollisionInstance;

class CollisionObject {
  public:
    CollisionObject(void *data = NULL);
    ~CollisionObject();

    void SearchCollisions(std::list<CollisionInstance> &collision_list);

    void AddCollisionGeom(CollisionMask*, geometry::GeometricShape*);
    void AddCollisionLogic(const CollisionMask* mask, CollisionLogic* logic);



    bool IsColliding(const CollisionObject*) const;

    // Getters and setters
    ugdk::Vector2D position() const { return position_; }
    void set_position(const ugdk::Vector2D &position) { position_ = position; }

    const geometry::GeometricShape* geom() const { return geom_.second; };

  private:
    void SetMask(CollisionMask*);


    ugdk::Vector2D position_;

    // Data that is sent to CollisionLogic::Handle
    void *data_;

    // TODO: Undo this simplification.
    // std::vector< CollisionGeom > geom_list_;
    CollisionGeom geom_;

    //CollisionMask* mask_;
    //geometry::GeometricShape* geom_;

    std::map<const CollisionMask*, CollisionLogic*> known_collisions_;
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONOBJECT_H_ */
