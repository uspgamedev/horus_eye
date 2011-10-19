#ifndef HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
#define HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_

#include <map>
#include <ugdk/action/sprite.h>
#include "game/utils/collisionobject.h"
#include "game/sprites/collision/collisionmask.h"
#include "game/sprites/collision/collisionobject.h"


namespace sprite {

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

// Classe WorldObject (abstrata)
// Representa um objeto do mundo (um bonequinho, uma magia, etc)
class WorldObject : public ugdk::Sprite {
  public:
    WorldObject();
    virtual ~WorldObject();

    // estado do objeto
    enum CollisionType { NO_COLLISION = 0x1, STATIC = 0x2, MOVEABLE = 0x4 };
    enum Status { STATUS_ACTIVE, STATUS_DYING, STATUS_DEAD };
    virtual CollisionType collision_type() const { return collision_type_; }
    virtual Status status() const { return status_; }
    virtual void Update(float dt);
    virtual void Render();
    virtual float light_radius(){ return light_radius_; }
    virtual void set_light_radius(float radius);

    // atributos
    virtual ugdk::Vector2D world_position() const {
        return bound_->position();
    }
    virtual void set_world_position(const ugdk::Vector2D& pos);
    virtual const utils::CollisionObject * bound() const { return bound_; }

    // tratamento de colisao
    virtual bool IsColliding(WorldObject* obj) const;

    /* virtual void CollidesWith(Wall* obj) {}
    virtual void CollidesWith(Door* obj) {}
    virtual void CollidesWith(Block* obj) {}
    virtual void CollidesWith(Creature* obj) {}
    virtual void CollidesWith(Hero* hero) {}
    virtual void CollidesWith(Explosion *explosion) {}
    virtual void CollidesWith(Projectile *projectile) {}
    virtual void CollidesWith(MummyProjectile *projectile) {}
    virtual void CollidesWith(Mummy *mummy) {}
    virtual void CollidesWith(Item *lifepotion) {} */

    //virtual void CollidesWith(WorldObject* obj) = 0;

    /* Should implement this as "obj->CollidesWith(this);".
       Reference: http://en.wikipedia.org/wiki/Double_dispatch */
    virtual void HandleCollision(WorldObject *obj) {
        this->CollidesWith(obj);
    }

    static const CollisionMask Collision() { return collision_; }
    virtual const CollisionMask collision() const { return WorldObject::Collision(); }
    
    void CollidesWith(WorldObject* obj) {
        CollisionMask mask = obj->collision();
        CollisionObject *col = known_collisions_[mask];
        if(col != NULL) col->Handle(obj);
    }

  protected:
    utils::CollisionObject *bound_;
    Status status_;
    CollisionType collision_type_;
    std::map<const CollisionMask,CollisionObject*> known_collisions_;

  private:
    static const CollisionMask collision_;
    float light_radius_;


    //friend void scene::World::Update(float);

};  // class WorldObject

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
