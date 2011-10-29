#ifndef HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
#define HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_

#include <map>
#include <list>
#include <ugdk/math/vector2D.h>
#include <ugdk/action/sprite.h>
#include "pyramidworks/collision/collisionobject.h"
#include <pyramidworks/collision/collisionmask.h>
#include <pyramidworks/collision/collisionlogic.h>

namespace sprite {

using pyramidworks::collision::CollisionMask;
using pyramidworks::collision::CollisionLogic;
using pyramidworks::collision::CollisionObject;

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
  DEFINE_COLLIDABLE
  public:
    WorldObject();
    virtual ~WorldObject();

    // Possible collision values. TODO explain better
    enum CollisionType { NO_COLLISION = 0x1, STATIC = 0x2, MOVEABLE = 0x4 };

    // Possible statuses. TODO explain better
    enum Status { STATUS_ACTIVE, STATUS_DYING, STATUS_DEAD };

    virtual CollisionType collision_type() const { return collision_type_; }
    virtual Status status() const { return status_; }

    // The BIG Awesome update method. TODO explain better
    virtual void Update(float dt);

    // The BIG Awesome drawable. TODO explain better
    virtual void Render();

    // The Light radius. TODO explain better
    virtual float light_radius() { return light_radius_; }
    virtual void set_light_radius(float radius);

    virtual ugdk::Vector2D world_position() const { return collision_object_->position(); }
    virtual void set_world_position(const ugdk::Vector2D& pos);

    virtual const CollisionObject* collision_object() const { return collision_object_; }
    
    virtual bool IsColliding(WorldObject* obj) const;

  protected:
    CollisionObject *collision_object_;
    Status status_;
    CollisionType collision_type_;

  private:
    float light_radius_;

};  // class WorldObject

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
