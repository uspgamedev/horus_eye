#include <iostream>
#include <ugdk/base/engine.h>
#include <ugdk/action/animation.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/drawable/sprite.h>
#include <pyramidworks/geometry/rect.h>
#include <pyramidworks/collision/collisionobject.h>

#include "wall.h"

#include "game/scenes/world.h"
#include "game/utils/constants.h"
#include "game/utils/imagefactory.h"
#include "game/utils/tile.h"


namespace sprite {

using namespace ugdk;
using namespace utils;
using namespace scene;

#define PI          3.1415926535897932384626433832795
#define TRANSPARENCY_DISTANCE 1.75

Wall::Wall(FlexibleSpritesheet* image) : tile_(NULL) {

    image->set_hotspot(Vector2D(Constants::WALL_HOTSPOT_X, Constants::WALL_HOTSPOT_Y));

    visible_frame_ = 0;
    transparent_frame_ = 1;
    dark_visible_frame_ = 5;
    dark_transparent_frame_ = 6;

    sprite_ = new ugdk::graphic::Sprite(image);
    sprite_->SetDefaultFrame(visible_frame_);

    node_->set_drawable(sprite_);

    INITIALIZE_COLLISION;
    SET_COLLISIONCLASS(Wall);
    SET_COLLISIONSHAPE(new pyramidworks::geometry::Rect(1.0, 1.0));
}
Wall::~Wall() {}

void Wall::set_type(WallType walltype) {
    wall_type_ = walltype;

    sprite_->SetDefaultFrame(visible_frame_);
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
            topleft = Vector2D(-TRANSPARENCY_DISTANCE/2, -TRANSPARENCY_DISTANCE * 0.45);
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

void Wall::Update(double delta_t) {
    WorldObject::Update(delta_t);
    World* world = WORLD();
    // Only use this if the walls are supposed to become not visible.
    //CheckType();
    if(!tile_)
        tile_ = Tile::GetFromWorldPosition(world->level_matrix(), world_position());

    if(world->hero() != NULL) {
        Vector2D distance = world->hero_world_object()->world_position() - world_position();
        if(transparency_square_.Contains(distance)) {
            if(tile_->visible())
                sprite_->SetDefaultFrame(transparent_frame_);
            else
                sprite_->SetDefaultFrame(dark_transparent_frame_);
        }
        else {
            if(tile_->visible())
                sprite_->SetDefaultFrame(visible_frame_);
            else
                sprite_->SetDefaultFrame(dark_visible_frame_);
        }
    }
}

}
