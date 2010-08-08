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
#define TRANSPARENCY_DISTANCE 1.5f

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
Wall::~Wall() {
    SelectAnimation(visible_animation_);
    delete transparent_animation_;
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

class Square {
  public:
    Vector2D top_left_, bot_right_;
    Square(Vector2D& top_left, Vector2D& bot_right) {
        top_left_ = top_left;
        bot_right_ = bot_right;
    }

    bool Contains(Vector2D pos) {
        return (top_left_.x <= pos.x && pos.x <= bot_right_.x)
                && (top_left_.y <= pos.y && pos.y <= bot_right_.y);
    }
};

void Wall::Update(float delta_t) {
    WorldObject::Update(delta_t);
    World* world = WORLD();
    if(world->hero() != NULL) {
        Vector2D topleft, botright(TRANSPARENCY_DISTANCE, TRANSPARENCY_DISTANCE);
        switch(wall_type_) {
        case BOTTOM:
            topleft = Vector2D(0, -TRANSPARENCY_DISTANCE);
            break;
        case RIGHT:
            topleft = Vector2D(-TRANSPARENCY_DISTANCE, 0);
            break;
        default:
            topleft = Vector2D(-TRANSPARENCY_DISTANCE/2, -TRANSPARENCY_DISTANCE/2);
            break;
        }
        Square transpquare(topleft, botright);

        Vector2D distance = world->hero()->world_position() - world_position();
        if(transpquare.Contains(distance))
            SelectAnimation(transparent_animation_);
        else
            SelectAnimation(visible_animation_);
    }
}

void Wall::HandleCollision(WorldObject* obj) {
    obj->CollidesWith(this);
}

}
