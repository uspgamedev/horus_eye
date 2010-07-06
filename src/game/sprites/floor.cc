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
#include <float.h>

using namespace framework;
namespace sprite {

#define FLOOR_WIDTH   71.0
#define FLOOR_HEIGHT  41.0

Floor::Floor() {
    Initialize(VIDEO_MANAGER()->LoadImage("data/images/chao41x41.png"));
    image()->set_frame_size(framework::Vector2D(FLOOR_WIDTH,FLOOR_HEIGHT));
    set_hotspot(Vector2D(FLOOR_WIDTH/2, FLOOR_HEIGHT/2));
    this->collision_type_ = NO_COLLISION;
}

void Floor::Update(float delta_t) {
    WorldObject::Update(delta_t);
    set_zindex(-FLT_MAX); // chao deve ficar a baixo de tudo/
}

}



