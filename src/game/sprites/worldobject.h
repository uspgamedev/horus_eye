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

// Classe WorldObject (abstrata)
// Representa um objeto do mundo (um bonequinho, uma magia, etc)
class WorldObject : public framework::Sprite {
  public:
    WorldObject();
    virtual ~WorldObject();

    // estado do objeto
    enum Status { STATUS_ACTIVE, STATUS_DYING, STATUS_DEAD };
    virtual Status status() const { return status_; }
    virtual void Update(float dt) { Sprite::Update(dt); }

    // atributos
    virtual float collision_radius() const { return collision_radius_; }
    virtual framework::Vector2D world_position() const { return world_position_; }
    virtual void set_world_position(const framework::Vector2D& pos) {
        world_position_ = pos;
    }

    // tratamento de colisao
    virtual bool IsColliding(WorldObject* obj) const;
    virtual void CollidesWith(WorldObject* obj) {}
    // tratamento de colisao
    void CollidesWithG(WorldObject* obj);

  protected:

    // atributos
    framework::Vector2D world_position_;
    float collision_radius_;
    Status status_;


    //friend void scene::World::Update(float);

};  // class WorldObject

}  // namespace sprite

#endif  // HORUSEYE_GAME_SPRITE_WORLDOBJECT_H_
