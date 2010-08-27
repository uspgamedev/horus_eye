//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/worldobject.h
// Definicao da classe WorldObject.
//

#ifndef HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
#define HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_

#include "../../framework/sprite.h"
#include "../utils/collisionobject.h"


namespace sprite {

class Hero;
class Mummy;
class Projectile;
class MummyProjectile;
class Wall;
class Door;
class Item;

// Classe WorldObject (abstrata)
// Representa um objeto do mundo (um bonequinho, uma magia, etc)
class WorldObject : public framework::Sprite {
  public:
    WorldObject();
    WorldObject(WorldObject* obj);
    virtual ~WorldObject();

    // estado do objeto
    enum CollisionType { NO_COLLISION = 0x1, STATIC = 0x2, MOVEABLE = 0x4 };
    enum Status { STATUS_ACTIVE, STATUS_DYING, STATUS_DEAD };
    virtual CollisionType collision_type() const { return collision_type_; }
    virtual Status status() const { return status_; }
    virtual void Update(float dt);
    virtual float light_radius(){ return light_radius_; }
    virtual void set_light_radius(float radius);

    // atributos
    virtual framework::Vector2D world_position() const {
        return bound_->position();
    }
    virtual void set_world_position(const framework::Vector2D& pos) {
        bound_->set_position(pos);
    }
    virtual const utils::CollisionObject * bound() const { return bound_; }

    // tratamento de colisao
    virtual bool IsColliding(WorldObject* obj) const;

    virtual void CollidesWith(Wall* obj) {}
    virtual void CollidesWith(Door* obj) {}
    virtual void CollidesWith(Hero* hero) {}
    virtual void CollidesWith(Projectile *projectile) {}
    virtual void CollidesWith(MummyProjectile *projectile) {}
    virtual void CollidesWith(Mummy *mummy) {}
    virtual void CollidesWith(Item *lifepotion) {}

    virtual void CollidesWith(WorldObject* obj) {}
    // tratamento de colisao
    virtual void HandleCollision(WorldObject* obj);

  protected:

    // atributos
    utils::CollisionObject *bound_;
    Status status_;
    CollisionType collision_type_;
    float light_radius_;


    //friend void scene::World::Update(float);

};  // class WorldObject

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
