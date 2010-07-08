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
#include "../../framework/vector2D.h"



namespace sprite {

class Hero;
class Mummy;
class Projectile;
class Wall;

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

    // atributos
    virtual float collision_radius() const { return collision_radius_; }
    virtual framework::Vector2D world_position() const { return world_position_; }
    virtual void set_world_position(const framework::Vector2D& pos) {
        world_position_ = pos;
    }

    // tratamento de colisao
    virtual bool IsColliding(WorldObject* obj) const;

    virtual void CollidesWith(Wall* obj) {}
    virtual void CollidesWith(Hero* hero) {}
    virtual void CollidesWith(Projectile *projectile) {}
    virtual void CollidesWith(Mummy *mummy) {}

    virtual void CollidesWith(WorldObject* obj) {}
    // tratamento de colisao
    virtual void HandleCollision(WorldObject* obj);

  protected:

    // atributos
    framework::Vector2D world_position_;
    float collision_radius_;
    Status status_;
    CollisionType collision_type_;


    //friend void scene::World::Update(float);

};  // class WorldObject

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
