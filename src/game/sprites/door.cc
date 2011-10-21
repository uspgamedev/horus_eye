#include "door.h"
#include <ugdk/base/engine.h>
#include <ugdk/graphic/image.h>
#include "game/scenes/world.h"
#include "game/sprites/creatures/hero.h"
#include "game/utils/rectobject.h"
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

Door::Door(ugdk::Image* image) {
    Initialize(image);
    set_hotspot(Vector2D(HOTSPOT_WIDTH, HOTSPOT_HEIGHT));
    this->collision_type_ = STATIC;
    bound_ = new RectObject(BOUND_WIDTH, BOUND_HEIGHT);

    Door::collision_;
    known_collisions_[Hero::Collision()] = new Collisions::Win(this);
}

COLLISION_IMPLEMENT(Door, Win, obj) {
    World *world = WORLD();
    if (world->CountRemainingEnemies() == 0)
        world->FinishLevel(LevelManager::FINISH_WIN);
}

}
