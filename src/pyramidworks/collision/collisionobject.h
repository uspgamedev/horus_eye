#ifndef PYRAMIDWORKS_COLLISION_COLLISIONOBJECT_H_
#define PYRAMIDWORKS_COLLISION_COLLISIONOBJECT_H_

#include <map>
#include <vector>
#include <ugdk/math/vector2D.h>

namespace pyramidworks {

namespace geometry {
class GeometricObject;
}

namespace collision {

class CollisionMask;
class CollisionLogic;

typedef std::pair<CollisionMask*, const geometry::GeometricObject*> CollisionGeom;

class CollisionObject {
  public:
    CollisionObject(void *data = NULL);
    ~CollisionObject();

    bool CollidesWith(CollisionObject*, const CollisionMask*);
    bool CollidesWith(CollisionObject* obj) { return CollidesWith(obj, obj->geom_.first); }

    ugdk::Vector2D position() const { return position_; }
    void set_position(const ugdk::Vector2D &position) { position_ = position; }

    bool IsColliding(const CollisionObject*) const;

    void AddCollisionGeom(CollisionMask*, geometry::GeometricObject*);
    void SetMask(CollisionMask*);

    void AddCollision(const CollisionMask* mask, CollisionLogic* logic) {
        known_collisions_[mask] = logic;
    }

    const geometry::GeometricObject* geom() const { return geom_.second; };

  private:
    ugdk::Vector2D position_;

    void *data_;

    // TODO: Undo this simplification.
    // std::vector< CollisionGeom > geom_list_;
    CollisionGeom geom_;

    //CollisionMask* mask_;
    //geometry::GeometricObject* geom_;

    std::map<const CollisionMask*, CollisionLogic*> known_collisions_;
};

} // namespace collision
} // namespace pyramidworks

#endif /* PYRAMIDWORKS_COLLISION_COLLISIONOBJECT_H_ */
