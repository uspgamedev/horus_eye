#include "door.h"
#include "ugdk/engine.h"
#include "ugdk/image.h"
#include "../scenes/world.h"
#include "../utils/rectobject.h"
#include "../utils/levelmanager.h"
#include "../utils/constants.h"
#include <iostream>

namespace sprite {

#define HOTSPOT_WIDTH   Constants::DOOR_HOTSPOT_WIDTH
#define HOTSPOT_HEIGHT  Constants::DOOR_HOTSPOT_HEIGHT
#define BOUND_WIDTH     Constants::DOOR_BOUND_WIDTH
#define BOUND_HEIGHT    Constants::DOOR_BOUND_HEIGHT

using namespace ugdk;
using namespace utils;
using namespace scene;

Door::Door(ugdk::Image* image) {
    Initialize(image);
    set_hotspot(Vector2D(HOTSPOT_WIDTH, HOTSPOT_HEIGHT));
    this->collision_type_ = STATIC;
    bound_ = new RectObject(BOUND_WIDTH, BOUND_HEIGHT);
}

void Door::CollidesWith(Hero *hero) {
    World *world = WORLD();
    if (!world->CountRemainingEnemies())
        world->FinishLevel(LevelManager::FINISH_WIN);
}

void Door::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

}
