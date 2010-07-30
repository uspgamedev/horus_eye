//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/sprites/wall.cc
// Implementacao da classe Wall
//

#include "wall.h"
#include "hero.h"
#include "../scenes/world.h"
#include "../utils/rectobject.h"
#include <iostream>


namespace sprite {

using namespace framework;
using namespace utils;
using namespace scene;

#define SQRT_2       1.4142135623730950488016887242097
#define WALL_WIDTH   106.0
#define WALL_HEIGHT  157.0
#define PI          3.1415926535897932384626433832795

Wall::Wall() {
    Initialize(VIDEO_MANAGER()->LoadImage("data/images/stoneblock2.png"));
    image()->set_frame_size(framework::Vector2D(WALL_WIDTH, WALL_HEIGHT));
    set_hotspot(Vector2D(WALL_WIDTH/2, WALL_HEIGHT - 30.5));
    visible_animation_ = new Animation(50, 0, -1);
    transparent_animation_ = new Animation(50, 1, -1);
    collision_type_ = STATIC;
    SelectAnimation(visible_animation_);
    bound_ = new RectObject(1.0f, 1.0f);
}

void Wall::set_type(WallType walltype) {
    wall_type_ = walltype;
    SelectAnimation(visible_animation_);
    delete transparent_animation_;
    int type;
    switch(wall_type_) {
    case MIDDLE:
        type = 4;
        break;
    case RIGHT:
        type = 3;
        break;
    case BOTTOM:
        type = 2;
        break;
    case BOTTOMRIGHT:
        type = 1;
        break;
    }
    transparent_animation_ = new Animation(50, type, -1);
}

void Wall::Update(float delta_t) {
    WorldObject::Update(delta_t);
    World* world = WORLD();
    if(world->hero() != NULL) {
        Vector2D distance = world->hero()->world_position() - world_position();
        float angle = distance.angle();
        /*if( world->hero()->world_position().y >= this->world_position().y - 1.5 &&
            world->hero()->world_position().y <= this->world_position().y + 1.5 &&
            world->hero()->world_position().x >= this->world_position().x  &&
            world->hero()->world_position().x <= this->world_position().x + 1.5 ) {*/
        if( distance.length() < 2.5f ) {
            if( (wall_type_ == BOTTOM && -PI/4.0 < angle && angle < PI/4.0) ||
                (wall_type_ == RIGHT && PI/4.0 < angle && angle < 3.0*PI/4.0) )
            SelectAnimation(transparent_animation_);
        } else {
            SelectAnimation(visible_animation_);
        }
    }
}

void Wall::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

}
