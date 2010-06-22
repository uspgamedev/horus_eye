//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/wall.h
// Definicao da classe Wall
//

#ifndef WALL_H_
#define WALL_H_

#include "../../framework/sprite.h"
#include "../../framework/engine.h"
#include "worldobject.h"

namespace sprite {

class Wall : public WorldObject {

  public:
    Wall() {
        Initialise(VIDEO_MANAGER()->LoadImage("data/images/wall0011.png"));
        image()->set_frame_size(framework::Vector2D(160,160));
    }

};

}

#endif /* WALL_H_ */
