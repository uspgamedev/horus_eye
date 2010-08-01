//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/wall.h
// Definicao da classe Wall
//

#ifndef HORUSEYE_GAME_SPRITE_WALL_H_
#define HORUSEYE_GAME_SPRITE_WALL_H_

#include "../../framework/sprite.h"
#include "../../framework/engine.h"
#include "worldobject.h"
#include "../../framework/animation.h"

namespace sprite {

class Wall : public WorldObject {

  public:
    enum WallType { MIDDLE, RIGHT, BOTTOM, BOTTOMRIGHT };

    Wall();
    ~Wall();
    virtual void Update(float dt);
    virtual void HandleCollision(WorldObject *);
    void set_type(WallType);

    WallType wall_type_;

  private:
    framework::Animation *visible_animation_, *transparent_animation_;
};

}

#endif /* WALL_H_ */
