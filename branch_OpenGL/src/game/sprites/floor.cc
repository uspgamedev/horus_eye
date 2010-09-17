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
#include "../utils/constants.h"
#include <float.h>

using namespace framework;
using namespace utils;
namespace sprite {

Floor::Floor(framework::Image* image) {
    Initialize(image);
    set_hotspot(Vector2D(Constants::FLOOR_HOTSPOT_X, Constants::FLOOR_HOTSPOT_Y));
    this->collision_type_ = NO_COLLISION;
    bound_ = new CircleObject(0.0f);
}

void Floor::Update(float delta_t) {
    WorldObject::Update(delta_t);
    set_zindex(-FLT_MAX); // chao deve ficar a baixo de tudo/
}

}



