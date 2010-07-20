//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/door.cc
// Implementacao da classe Door.
//

#include "door.h"
#include "../../framework/engine.h"
#include "../scenes/world.h"
#include "../utils/rectobject.h"
#include "../utils/constants.h"
#include <iostream>

namespace sprite {

#define DOOR_WIDTH      Constants::DOOR_SPRITE_WIDTH
#define DOOR_HEIGHT     Constants::DOOR_SPRITE_HEIGHT
#define HOTSPOT_HEIGHT  Constants::DOOR_HOTSPOT_HEIGHT
#define BOUND_WIDTH     Constants::DOOR_BOUND_WIDTH
#define BOUND_HEIGHT    Constants::DOOR_BOUND_HEIGHT

using namespace framework;
using namespace utils;
using namespace scene;

Door::Door() {
    Initialize(VIDEO_MANAGER()->LoadImage("data/images/stairs.png"));
    image()->set_frame_size(framework::Vector2D(DOOR_WIDTH, DOOR_HEIGHT));
    set_hotspot(Vector2D(DOOR_WIDTH/2.0, DOOR_HEIGHT - HOTSPOT_HEIGHT));
    this->collision_type_ = STATIC;
    bound_ = new RectObject(BOUND_WIDTH, BOUND_HEIGHT);
}

void Door::CollidesWith(Hero *hero) {
    World *world = ((World*)Engine::reference()->CurrentScene());
    if (!world->CountRemainingEnemies())
        world->FinishLevel(true);
}

void Door::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

}