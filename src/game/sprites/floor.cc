//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/floor.cc
// Implementacao da classe Floor
//
//
#include "floor.h"
#include "../../framework/videomanager.h"
#include "../../framework/engine.h"
#include "../utils/circleobject.h"
#include <float.h>

using namespace framework;
using namespace utils;
namespace sprite {

#define FLOOR_WIDTH   106.0
#define FLOOR_HEIGHT  61.0

Floor::Floor() {
    Initialize(VIDEO_MANAGER()->LoadImage("data/images/chao106x106.png"));
    image()->set_frame_size(framework::Vector2D(FLOOR_WIDTH,FLOOR_HEIGHT));
    set_hotspot(Vector2D(FLOOR_WIDTH/2, FLOOR_HEIGHT/2));
    this->collision_type_ = NO_COLLISION;
    bound_ = new CircleObject(0.0f);
}

void Floor::Update(float delta_t) {
    WorldObject::Update(delta_t);
    set_zindex(-FLT_MAX); // chao deve ficar a baixo de tudo/
}

}



