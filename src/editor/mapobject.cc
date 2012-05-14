#include <iostream>
#include <ugdk/graphic/drawable/sprite.h>
#include <ugdk/graphic/drawable/solidrectangle.h>
#include <ugdk/graphic/spritesheet/flexiblespritesheet.h>
#include <ugdk/graphic/node.h>
#include "mapobject.h"
#include "game/utils/imagefactory.h"
#include "game/utils/constants.h"
#include "game/utils/tile.h"
#include "game/scenes/world.h"


using namespace ugdk;
using utils::Constants;

namespace editor {

#define SET_IMG_TO(img, val) if(img == NULL) { img = val; }

const double MapObject::TileSize = 10.0;

MapObject::MapObject(int i, int j, char type, int level_width, int level_height) : x_(j), y_(i), type_(type), is_in_fill_(false) {
    tile_image_ = new ugdk::graphic::SolidRectangle(Vector2D(TileSize, TileSize));

    utils::ImageFactory img_factory;
    sprite_image_ = NULL;
    switch(type_) {
    case WALL:
        tile_image_->set_color(Color(0.25, 0.25, 0.25));
        if(sprite_image_ == NULL) {
            sprite_image_ = img_factory.WallImage();
        }
        break;
    case DOOR:
        tile_image_->set_color(Color(0.0, 0.5, 0.5));
        if(sprite_image_ == NULL) {
            sprite_image_ = img_factory.DoorImage();
        }
        break;
    case ENTRY:
        tile_image_->set_color(Color(0.33, 0.33, 0.25));
        if(sprite_image_ == NULL) {
            sprite_image_ = img_factory.EntryImage();
        }
        break;
    case MUMMY:
    case STANDING_MUMMY:
        if(sprite_image_ == NULL) {
            sprite_image_ = img_factory.MummyImage();
        }
    case RANGED_MUMMY:
    case STANDING_RANGED_MUMMY:
        if(sprite_image_ == NULL) {
            sprite_image_ = img_factory.RangedMummyImage();
        }
    case BIG_MUMMY:
    case STANDING_BIG_MUMMY:
        if(sprite_image_ == NULL) {
            sprite_image_ = img_factory.BigMummyImage();
        }
    case PHARAOH:
    case STANDING_PHARAOH:
        tile_image_->set_color(Color(1.0, 0.0, 0.0));
        if(sprite_image_ == NULL) {
            sprite_image_ = img_factory.PharaohImage();
        }
        break;
    case HERO:
        tile_image_->set_color(Color(1.0, 1.0, 0.0));
        if(sprite_image_ == NULL) {
            sprite_image_ = img_factory.HeroImage();
        }
        break;
    case FLOOR:
        tile_image_->set_color(Color(0.5, 0.5, 0.5));
        if(sprite_image_ == NULL) {
            sprite_image_ = img_factory.FloorImage();
        }
        break;
    case POTIONL:
        if(sprite_image_ == NULL) {
            sprite_image_ = NULL; //img_factory.LifePotionImage();
        }
    case POTIONM:
        if(sprite_image_ == NULL) {
            sprite_image_ = NULL; //img_factory.ManaPotionImage();
        }
    case POTIONS:
        tile_image_->set_color(Color(0.0, 1.0, 0.5));
        if(sprite_image_ == NULL) {
            sprite_image_ = NULL; //img_factory.SightPotionImage();
        }
        break;
    case BLOCK:
        tile_image_->set_color(Color(0.50, 0.25, 0.25));
        if(sprite_image_ == NULL) {
            sprite_image_ = img_factory.WallImage();
        }
        //{   Vector2D new_size(sprite_image_->render_size().x, sprite_image_->render_size().y * 0.7);
        //    set_size(new_size); }
        break;
    case BUTTON:
        tile_image_->set_color(Color(0.5, 0.5, 0.8));
        if(sprite_image_ == NULL) {
            sprite_image_ = img_factory.TileSwitchImage();
        }
        break;
    default:
        tile_image_->set_color(Color(1.0, 1.0, 1.0));
        if(sprite_image_ == NULL) {
            sprite_image_ = NULL;
        }
        break;
    }
	if(sprite_image_)
		isometric_node_ = new ugdk::graphic::Node(new ugdk::graphic::Sprite(sprite_image_));
	else
		isometric_node_ = new ugdk::graphic::Node(new ugdk::graphic::SolidRectangle(Vector2D(TileSize, TileSize)));
	
    Vector2D position ((double)x_, (double)(level_height - y_ - 1));
	isometric_node_->modifier()->set_offset(scene::World::FromWorldCoordinates(position));
	if(type_ == FLOOR || type_ == EMPTY)
        isometric_node_->set_zindex(-1337000.0);
    else
        isometric_node_->set_zindex(scene::World::FromWorldLinearCoordinates(position).y);

	tile_node_ = new ugdk::graphic::Node(tile_image_);
	tile_node_->modifier()->set_offset(Vector2D(x_*TileSize, y_*TileSize));
}

MapObject::~MapObject() {
	delete isometric_node_;
	delete tile_node_;
}

void MapObject::Update(double delta_t) {}

void MapObject::Select(bool on) {
	isometric_node_->modifier()->set_alpha(on ? 0.5 : 1.0);
	tile_node_->modifier()->set_alpha(on ? 0.5 : 1.0);
}

}
