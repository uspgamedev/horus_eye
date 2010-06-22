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

using namespace framework;
namespace sprite {


Floor::Floor() {
    Initialise(VIDEO_MANAGER()->LoadImage("data/images/chao41x41.png"));
    image()->set_frame_size(framework::Vector2D(71,41));
    set_hotspot(Vector2D(35.5, 20.5));
}

void Floor::Update(float delta_t) {
    WorldObject::Update(delta_t);
    set_zindex(0); // chao deve ficar a baixo de tudo
}

}



