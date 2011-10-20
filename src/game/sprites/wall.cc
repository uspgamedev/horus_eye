#include <iostream>
#include <ugdk/base/engine.h>
#include <ugdk/action/animation.h>
#include "wall.h"
#include "hero.h"
#include "../scenes/world.h"
#include "../utils/rectobject.h"
#include "../utils/constants.h"
#include "../utils/imagefactory.h"
#include "../utils/tile.h"


namespace sprite {

using namespace ugdk;
using namespace utils;
using namespace scene;

INITIALIZE_COLLIDABLE_NODE(Wall, WorldObject);

#define PI          3.1415926535897932384626433832795
#define TRANSPARENCY_DISTANCE 1.75f

Wall::Wall(Image* image) {
    Initialize(image);
    set_hotspot(Vector2D(Constants::WALL_HOTSPOT_X, Constants::WALL_HOTSPOT_Y));

    visible_frame_ = 0;
    transparent_frame_ = 1;
    dark_visible_frame_ = 5;
    dark_transparent_frame_ = 6;
    SetDefaultFrame(visible_frame_);
    collision_type_ = STATIC;
    bound_ = new RectObject(1.0f, 1.0f);
    tile_ = NULL;
}
Wall::~Wall() {}

void Wall::set_type(WallType walltype) {
    wall_type_ = walltype;

    SetDefaultFrame(visible_frame_);
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
    transparent_frame_ = type;
    dark_transparent_frame_ = 5 + type;
    transparency_square_ = Square(topleft, botright);
}

bool IsWall(Tile *tile) {
    return tile->visible()
           && (tile->object() == WALL || tile->object() == ENTRY);
}

void Wall::CheckType() {
    GameMap &map = WORLD()->level_matrix();

    if(!tile_)
        tile_ = Tile::GetFromWorldPosition(map, world_position());

    Tile *left = tile_->Left(map),
         *down = tile_->Down(map);

    if(left && IsWall(left)) {
        if(down && IsWall(down)) {
            set_type(MIDDLE);
        } else {
            set_type(RIGHT);
        }
    } else {
        if(down && IsWall(down)) {
            set_type(BOTTOM);
        } else {
            set_type(BOTTOMRIGHT);
        }
    }
}

void Wall::Update(float delta_t) {
    WorldObject::Update(delta_t);
    World* world = WORLD();
    // Only use this if the walls are supposed to become not visible.
    //CheckType();
    if(!tile_)
        tile_ = Tile::GetFromWorldPosition(world->level_matrix(), world_position());

    if(world->hero() != NULL) {
        Vector2D distance = world->hero()->world_position() - world_position();
        if(transparency_square_.Contains(distance)) {
            if(tile_->visible())
                SetDefaultFrame(transparent_frame_);
            else
                SetDefaultFrame(dark_transparent_frame_);
        }
        else {
            if(tile_->visible())
                SetDefaultFrame(visible_frame_);
            else
                SetDefaultFrame(dark_visible_frame_);
        }
    }
}

}
