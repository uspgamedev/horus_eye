#ifndef HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
#define HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_

#include <list>
#include <ugdk/math/vector2D.h>
#include <ugdk/action/sprite.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>
#include <pyramidworks/collision/collisionclass.h>
#include <pyramidworks/collision/collisionlogic.h>

namespace sprite {

using pyramidworks::collision::CollisionClass;
using pyramidworks::collision::CollisionLogic;
using pyramidworks::collision::CollisionObject;

#define INITIALIZE_COLLISION { if(collision_object_ == NULL) collision_object_ = new pyramidworks::collision::CollisionObject(this); }

#define SET_COLLISIONCLASS(CLASS)        { collision_object_->set_collision_class(GET_COLLISIONMASK(CLASS)); }
#define SET_COLLISIONSHAPE(SHAPE)        set_shape(SHAPE);
#define ADD_COLLISIONLOGIC(CLASS, LOGIC) { collision_object_->AddCollisionLogic(GET_COLLISIONMASK(CLASS), LOGIC); }

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

class WorldObject : public ugdk::Sprite {
  
  public:
    WorldObject();
    virtual ~WorldObject();

    // Possible statuses. TODO explain better
    enum Status { STATUS_ACTIVE, STATUS_DYING, STATUS_DEAD };

    virtual Status status() const { return status_; }

    // The BIG Awesome update method. TODO explain better
    virtual void Update(float dt);

    // The BIG Awesome drawable. TODO explain better
    virtual void Render();

    // The Light radius. TODO explain better
    virtual float light_radius() { return light_radius_; }
    virtual void set_light_radius(float radius);

    virtual ugdk::Vector2D world_position() const { return world_position_; }
    virtual void set_world_position(const ugdk::Vector2D& pos);

    virtual CollisionObject* collision_object() const { return collision_object_; }

    void set_shape(pyramidworks::geometry::GeometricShape* shape);

  protected:
    CollisionObject *collision_object_;
    Status status_;
    ugdk::Vector2D world_position_;

    std::string identifier_;

  private:
    float light_radius_;

};  // class WorldObject

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
