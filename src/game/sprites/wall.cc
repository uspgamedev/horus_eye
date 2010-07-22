//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/wall.cc
// Implementacao da classe Wall
//

#include "wall.h"
#include "../utils/rectobject.h"
#include <iostream>


namespace sprite {

using namespace framework;
using namespace utils;

#define SQRT_2       1.4142135623730950488016887242097
#define WALL_WIDTH   106.0
#define WALL_HEIGHT  157.0

Wall::Wall() {
        Initialize(VIDEO_MANAGER()->LoadImage("data/images/stoneblock.png"));
        image()->set_frame_size(framework::Vector2D(WALL_WIDTH, WALL_HEIGHT));
        set_hotspot(Vector2D(WALL_WIDTH/2, WALL_HEIGHT - 30.5));
        this->collision_type_ = STATIC;
        bound_ = new RectObject(1.0f, 1.0f);
}

void Wall::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

}
