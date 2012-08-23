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
#include "game/sprites/worldobject.h"
#include "game/utils/tile.h"


namespace component {

using namespace ugdk;
using namespace utils;
using namespace scene;

#define PI          3.1415926535897932384626433832795
#define TRANSPARENCY_DISTANCE 1.75

Wall::Wall(sprite::WorldObject* owner, ugdk::graphic::Spritesheet* sheet) 
    : owner_(owner), tile_(NULL) {

    visible_frame_ = 0;
    transparent_frame_ = 1;
    dark_visible_frame_ = 5;
    dark_transparent_frame_ = 6;

    sprite_ = new ugdk::graphic::Sprite(sheet);
    sprite_->SetDefaultFrame(visible_frame_);

    owner->node()->set_drawable(sprite_);
}
Wall::~Wall() {
    tile_->set_object(' ');
}

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
        tile_ = Tile::GetFromWorldPosition(map, owner_->world_position());

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
    World* world = WORLD();
    // Only use this if the walls are supposed to become not visible.

    if(!tile_)
        tile_ = Tile::GetFromWorldPosition(world->level_matrix(), owner_->world_position());

    CheckType();

    if(world->hero() != NULL) {
        Vector2D distance = world->hero_world_object()->world_position() - owner_->world_position();
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
