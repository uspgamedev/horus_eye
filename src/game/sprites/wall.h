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

namespace sprite {

class Wall : public WorldObject {

  public:
    Wall();
    virtual void HandleCollision(WorldObject *);
};

}

#endif /* WALL_H_ */
