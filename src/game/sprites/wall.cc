//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/wall.cc
// Implementacao da classe Wall
//

#include "wall.h"

namespace sprite {

using namespace framework;

Wall::Wall() {
        Initialise(VIDEO_MANAGER()->LoadImage("data/images/wall0011.png"));
        image()->set_frame_size(framework::Vector2D(160,160));
        set_hotspot(Vector2D(36, 55));
}

}
