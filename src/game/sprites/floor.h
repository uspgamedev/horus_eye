//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/floor.h
// Definicao da classe Floor
//

#ifndef FLOOR_H_
#define FLOOR_H_

#include "worldobject.h"

namespace sprite {

class Floor : public WorldObject {

  public:

    Floor() {
        Initialise(VIDEO_MANAGER()->LoadImage("data/images/chao41x41.png"));
        image()->set_frame_size(framework::Vector2D(71,41));
        set_hotspot(Vector2D(35.5, 20.5));
    }



};

}

#endif /* FLOOR_H_ */
