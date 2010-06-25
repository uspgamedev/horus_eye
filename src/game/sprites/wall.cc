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

#define SQRT_2        1.4142135623730950488016887242097
#define WALL_WIDTH   71.0
#define WALL_HEIGHT  74.0

Wall::Wall() {
        Initialize(VIDEO_MANAGER()->LoadImage("data/images/block.png"));
        image()->set_frame_size(framework::Vector2D(WALL_WIDTH, WALL_HEIGHT));
        set_hotspot(Vector2D(WALL_WIDTH/2, WALL_HEIGHT - 20.5));
        this->collision_radius_ = SQRT_2/2;
}

}
