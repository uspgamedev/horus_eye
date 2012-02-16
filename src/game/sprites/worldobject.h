#ifndef HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
#define HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_

#include <string>
#include <ugdk/math/vector2D.h>
#include <ugdk/action/entity.h>

namespace ugdk {
    namespace graphic {
        class Node;
    }
}

namespace pyramidworks {
    namespace geometry {
        class GeometricShape;
    }
    namespace collision {
        class CollisionObject;
    }
}

namespace sprite {

#define INITIALIZE_COLLISION { if(collision_object_ == NULL) collision_object_ = new pyramidworks::collision::CollisionObject(this); }

#define SET_COLLISIONCLASS(CLASS)        { collision_object_->InitializeCollisionClass(#CLASS); }
#define SET_COLLISIONSHAPE(SHAPE)        set_shape(SHAPE);
#define ADD_COLLISIONLOGIC(CLASS, LOGIC) { collision_object_->AddCollisionLogic(#CLASS, LOGIC); }

class Creature;
class Hero;
class Mummy;
class Projectile;
class Explosion;
class MummyProjectile;
class Wall;
class Door;
class Block;
class Item;

class WorldObject : public ugdk::Entity {
  public:
    WorldObject();
    virtual ~WorldObject();

    // Possible statuses. TODO explain better
    enum Status { STATUS_ACTIVE, STATUS_DYING, STATUS_DEAD };

    virtual Status status() const { return status_; }

    // The BIG Awesome update method. TODO explain better
    virtual void Update(double dt);

    bool is_active() const { return status_ == STATUS_ACTIVE; }
    virtual void Die() { status_ = STATUS_DEAD; }
    virtual void StartToDie();

    // The Light radius. TODO explain better
    virtual double light_radius() { return light_radius_; }
    virtual void set_light_radius(double radius);

    virtual ugdk::Vector2D world_position() const { return world_position_; }
    virtual void set_world_position(const ugdk::Vector2D& pos);

    ugdk::graphic::Node* node() { return node_; }
    const ugdk::graphic::Node* node() const { return node_; }

    virtual pyramidworks::collision::CollisionObject* collision_object() const { return collision_object_; }

    void set_shape(pyramidworks::geometry::GeometricShape* shape);

  protected:
    pyramidworks::collision::CollisionObject *collision_object_;
    Status status_;
    ugdk::Vector2D world_position_;
    ugdk::graphic::Node* node_;

    std::string identifier_;

  private:
    double light_radius_;

};  // class WorldObject

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
