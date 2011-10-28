#include <iostream>
#include "mapobject.h"
#include "../game/utils/imagefactory.h"
#include "../game/utils/constants.h"
#include "../game/scenes/world.h"

using namespace ugdk;
using utils::Constants;

namespace editor {

#define SET_IMG_TO(img, val) if(img == NULL) { img = val; }

const float MapObject::TileSize = 10.0f;

MapObject::MapObject(int i, int j, char type, int level_width, int level_height) : Sprite(), x_(j), y_(i), type_(type), is_in_fill_(false) {
    tile_image_ = new Image();
    tile_image_->set_render_size(Vector2D(TileSize, TileSize));
    utils::ImageFactory img_factory;
    sprite_image_ = NULL;
    switch(type_) {
    case WALL:
        tile_image_->set_color(Image::CreateColor(0.25f, 0.25f, 0.25f));
        if(sprite_image_ == NULL) {
            Initialize(sprite_image_ = img_factory.WallImage());
            set_hotspot(Vector2D(Constants::WALL_HOTSPOT_X, Constants::WALL_HOTSPOT_Y));
        }
        break;
    case DOOR:
        tile_image_->set_color(Image::CreateColor(0.0f, 0.5f, 0.5f));
        if(sprite_image_ == NULL) {
            Initialize(sprite_image_ = img_factory.DoorImage());
            set_hotspot(Vector2D(Constants::DOOR_HOTSPOT_WIDTH, Constants::DOOR_HOTSPOT_HEIGHT));
        }
        break;
    case ENTRY:
        tile_image_->set_color(Image::CreateColor(0.33f, 0.33f, 0.25f));
        if(sprite_image_ == NULL) {
            Initialize(sprite_image_ = img_factory.EntryImage());
            set_hotspot(Vector2D(Constants::WALL_HOTSPOT_X, Constants::WALL_HOTSPOT_Y));
        }
        break;
    case MUMMY:
    case STANDING_MUMMY:
        if(sprite_image_ == NULL) {
            Initialize(sprite_image_ = img_factory.MummyImage());
            set_hotspot(Vector2D(sprite_image_->frame_size().x / 2.0f, sprite_image_->frame_size().y*6.0f / 7.0f));
        }
    case RANGED_MUMMY:
    case STANDING_RANGED_MUMMY:
        if(sprite_image_ == NULL) {
            Initialize(sprite_image_ = img_factory.RangedMummyImage());
            set_hotspot(Vector2D(sprite_image_->frame_size().x / 2.0f, sprite_image_->frame_size().y*6.0f / 7.0f));
        }
    case BIG_MUMMY:
    case STANDING_BIG_MUMMY:
        if(sprite_image_ == NULL) {
            Initialize(sprite_image_ = img_factory.BigMummyImage());
            set_hotspot(Vector2D(sprite_image_->frame_size().x / 2.0f, sprite_image_->frame_size().y*6.0f / 7.0f));
        }
    case PHARAOH:
    case STANDING_PHARAOH:
        tile_image_->set_color(Image::CreateColor(1.0f, 0.0f, 0.0f));
        if(sprite_image_ == NULL) {
            Initialize(sprite_image_ = img_factory.PharaohImage());
            set_hotspot(Vector2D(sprite_image_->frame_size().x / 2.0f, sprite_image_->frame_size().y*6.0f / 7.0f));
        }
        break;
    case HERO:
        tile_image_->set_color(Image::CreateColor(1.0f, 1.0f, 0.0f));
        if(sprite_image_ == NULL) {
            Initialize(sprite_image_ = img_factory.HeroImage());
            set_hotspot(Vector2D(Constants::HERO_HOTSPOT_X, Constants::HERO_HOTSPOT_Y));
        }
        break;
    case FLOOR:
        tile_image_->set_color(Image::CreateColor(0.5f, 0.5f, 0.5f));
        if(sprite_image_ == NULL) {
            Initialize(sprite_image_ = img_factory.FloorImage());
            set_hotspot(Vector2D(Constants::FLOOR_HOTSPOT_X, Constants::FLOOR_HOTSPOT_Y));
        }
        break;
    case POTIONL:
        if(sprite_image_ == NULL) {
            Initialize(sprite_image_ = img_factory.LifePotionImage());
            set_hotspot(Vector2D(Constants::POTION_SPRITE_CENTER_X, Constants::POTION_SPRITE_CENTER_Y + Constants::POTION_HEIGHT));
        }
    case POTIONM:
        if(sprite_image_ == NULL) {
            Initialize(sprite_image_ = img_factory.ManaPotionImage());
            set_hotspot(Vector2D(Constants::POTION_SPRITE_CENTER_X, Constants::POTION_SPRITE_CENTER_Y + Constants::POTION_HEIGHT));
        }
    case POTIONS:
        tile_image_->set_color(Image::CreateColor(0.0f, 1.0f, 0.5f));
        if(sprite_image_ == NULL) {
            Initialize(sprite_image_ = img_factory.SightPotionImage());
            set_hotspot(Vector2D(Constants::POTION_SPRITE_CENTER_X, Constants::POTION_SPRITE_CENTER_Y + Constants::POTION_HEIGHT));
        }
        break;
    default:
        tile_image_->set_color(Image::CreateColor(1.0f, 1.0f, 1.0f));
        if(sprite_image_ == NULL) {
            sprite_image_ = tile_image_ = new Image();
            Initialize(sprite_image_);
        }
        break;
    }
    Vector2D position ((float)x_, (float)(level_height - y_ - 1));
    set_position(scene::World::FromWorldCoordinates(position));
    if(type_ == FLOOR || type_ == EMPTY)
        set_zindex(-1337000.0f);
    else
        set_zindex(scene::World::FromWorldLinearCoordinates(position).y);
}

MapObject::~MapObject() {
    delete tile_image_;
}

void MapObject::Update(float delta_t) {
    Sprite::Update(delta_t);
}

void MapObject::Render2D(ugdk::Vector2D offset, float scale) {
	tile_image_->DrawTo(Vector2D(x_*TileSize, y_*TileSize) * scale - offset, 0, 0, color(), alpha(), tile_image_->frame_size() * scale);
}

void MapObject::Select(bool on) {
    set_alpha(on ? 0.5f : 1.0f);
}

}
