#include <ugdk/base/engine.h>
#include <ugdk/graphic/image.h>
#include <pyramidworks/geometry/rect.h>

#include "door.h"

#include "game/scenes/world.h"
#include "game/utils/levelmanager.h"
#include "game/utils/constants.h"

namespace sprite {

INITIALIZE_COLLIDABLE_NODE(Door, Wall);

#define HOTSPOT_WIDTH   Constants::DOOR_HOTSPOT_WIDTH
#define HOTSPOT_HEIGHT  Constants::DOOR_HOTSPOT_HEIGHT
#define BOUND_WIDTH     Constants::DOOR_BOUND_WIDTH
#define BOUND_HEIGHT    Constants::DOOR_BOUND_HEIGHT

using namespace ugdk;
using namespace utils;
using namespace scene;

COLLISION_DIRECT(World*, WinCollision, obj) {
    if (data_->CountRemainingEnemies() == 0)
        data_->FinishLevel(LevelManager::FINISH_WIN);
}

Door::Door(ugdk::Image* image) {
    Initialize(image);
    set_hotspot(Vector2D(HOTSPOT_WIDTH, HOTSPOT_HEIGHT));

    INITIALIZE_COLLISION;
    SET_COLLISIONCLASS(Wall);
    SET_COLLISIONSHAPE(new pyramidworks::geometry::Rect(BOUND_WIDTH, BOUND_HEIGHT));
    ADD_COLLISIONLOGIC(Hero, new WinCollision(WORLD()));
}

}
