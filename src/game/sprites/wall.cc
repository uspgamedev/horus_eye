#include <iostream>
#include "../../framework/engine.h"
#include "../../framework/animation.h"
#include "wall.h"
#include "hero.h"
#include "../scenes/world.h"
#include "../utils/rectobject.h"
#include "../utils/constants.h"
#include "../utils/imagefactory.h"


namespace sprite {

using namespace framework;
using namespace utils;
using namespace scene;

#define SQRT_2       1.4142135623730950488016887242097
#define WALL_WIDTH   106.0
#define WALL_HEIGHT  157.0
#define PI          3.1415926535897932384626433832795
#define TRANSPARENCY_DISTANCE 1.5f

Wall::Wall() {
    ImageFactory image_factory;
    Initialize(image_factory.WallImage());
    set_hotspot(Vector2D(Constants::WALL_HOTSPOT_X, Constants::WALL_HOTSPOT_Y));
    visible_animation_ = new Animation(50, 0, -1);
    transparent_animation_ = new Animation(50, 1, -1);
    collision_type_ = STATIC;
    SelectAnimation(visible_animation_);
    bound_ = new RectObject(1.0f, 1.0f);
}
Wall::~Wall() {
    SelectAnimation(visible_animation_);
    delete transparent_animation_;
}

void Wall::set_type(WallType walltype) {
    wall_type_ = walltype;
    SelectAnimation(visible_animation_);
    delete transparent_animation_;
    int type = 1;
    Vector2D topleft, botright(TRANSPARENCY_DISTANCE, TRANSPARENCY_DISTANCE);
    switch(wall_type_) {
        case MIDDLE:
            type = 4;
            topleft = Vector2D(-TRANSPARENCY_DISTANCE/2, -TRANSPARENCY_DISTANCE/2);
            break;
        case RIGHT:
            type = 3;
            topleft = Vector2D(-TRANSPARENCY_DISTANCE, 0);
            break;
        case BOTTOM:
            type = 2;
            topleft = Vector2D(0, -TRANSPARENCY_DISTANCE);
            break;
        case BOTTOMRIGHT:
            type = 1;
            topleft = Vector2D(-TRANSPARENCY_DISTANCE/2, -TRANSPARENCY_DISTANCE * 0.45f);
            break;
    }
    transparent_animation_ = new Animation(50, type, -1);
    transparency_square_ = Square(topleft, botright);
}

void Wall::Update(float delta_t) {
    WorldObject::Update(delta_t);
    World* world = WORLD();
    if(world->hero() != NULL) {
        Vector2D distance = world->hero()->world_position() - world_position();
        if(transparency_square_.Contains(distance))
            SelectAnimation(transparent_animation_);
        else
            SelectAnimation(visible_animation_);
    }
}

void Wall::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

}
