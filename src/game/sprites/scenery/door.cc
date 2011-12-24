#include <ugdk/base/engine.h>
#include <ugdk/graphic/image.h>
#include <ugdk/action/sprite.h>
#include <pyramidworks/geometry/rect.h>

#include "door.h"

#include "game/scenes/world.h"
#include "game/utils/levelmanager.h"
#include "game/utils/constants.h"

namespace sprite {

#define HOTSPOT_WIDTH   utils::Constants::DOOR_HOTSPOT_WIDTH
#define HOTSPOT_HEIGHT  utils::Constants::DOOR_HOTSPOT_HEIGHT
#define BOUND_WIDTH     utils::Constants::DOOR_BOUND_WIDTH
#define BOUND_HEIGHT    utils::Constants::DOOR_BOUND_HEIGHT

using ugdk::Vector2D;

COLLISION_DIRECT(scene::World*, WinCollision, obj) {
    if (data_->CountRemainingEnemies() == 0 && data_->num_button_not_pressed().Get() == 0)
        data_->FinishLevel(utils::LevelManager::FINISH_WIN);
}

Door::Door(ugdk::FlexibleSpritesheet* image, scene::World *world) {
    image->set_hotspot(Vector2D(HOTSPOT_WIDTH, HOTSPOT_HEIGHT));
    node_->set_drawable(new ugdk::Sprite(image));

    INITIALIZE_COLLISION;
    SET_COLLISIONCLASS(Wall);
    SET_COLLISIONSHAPE(new pyramidworks::geometry::Rect(BOUND_WIDTH, BOUND_HEIGHT));
    ADD_COLLISIONLOGIC(Hero, new WinCollision(world));
}

}
