//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/imagefactory.h
// Definicao da classe ImageFactory.
//

#include "imagefactory.h"
#include "../scenes/world.h"

namespace utils {
    ImageFactory::ImageFactory(){
        hero_image_ = VIDEO_MANAGER()->LoadImage("data/images/sprite-sheet_MOD3.png");
        hero_image_->set_frame_size(Vector2D(110, 110));

        mummy_image_ = VIDEO_MANAGER()->LoadImage("data/images/sprite-sheet_MOD3_bw.png");
        mummy_image_->set_frame_size(Vector2D(110, 110));

        ranged_mummy_image_ = VIDEO_MANAGER()->LoadImage("data/images/sprite-sheet_MOD3_red.png");
        ranged_mummy_image_->set_frame_size(Vector2D(110, 110));

        big_mummy_image_ = VIDEO_MANAGER()->LoadImage("data/images/big_mummy_193x193.png");
        big_mummy_image_->set_frame_size(Vector2D(193, 193));

        projectile_image_ = VIDEO_MANAGER()->LoadImage("data/images/fire_ball.png");
        projectile_image_->set_frame_size( Vector2D(32, 32) );

        life_potion_image_ = VIDEO_MANAGER()->LoadImage("data/images/life_potion.png");
        life_potion_image_->set_frame_size(Vector2D(20, 20));

        mana_potion_image_ = VIDEO_MANAGER()->LoadImage("data/images/life_potion.png");
        mana_potion_image_->set_frame_size(Vector2D(20, 20));

        door_image_ = VIDEO_MANAGER()->LoadImage("data/images/stairs3.png");
        door_image_->set_frame_size(Vector2D(153, 109));

        floor_image_ = VIDEO_MANAGER()->LoadImage("data/images/chao106x106.png");
        floor_image_->set_frame_size(Vector2D(106,61));

        wall_image_ = VIDEO_MANAGER()->LoadImage("data/images/stoneblock2.png");
        wall_image_->set_frame_size(Vector2D(106, 157));
    }
    Image* ImageFactory::HeroImage(){
        return hero_image_;
    }
    Image* ImageFactory::BigMummyImage(){
        return big_mummy_image_;
    }
    Image* ImageFactory::MummyImage(){
        return mummy_image_;
    }
    Image* ImageFactory::RangedMummyImage(){
        return ranged_mummy_image_;
    }
    Image* ImageFactory::ProjectileImage() {
        return projectile_image_;
    }
    Image* ImageFactory::LifePotionImage() {
        return life_potion_image_;
    }
    Image* ImageFactory::ManaPotionImage() {
        return mana_potion_image_;
    }
    Image* ImageFactory::DoorImage() {
        return door_image_;
    }
    Image* ImageFactory::FloorImage() {
        return floor_image_;
    }
    Image* ImageFactory::WallImage() {
        return wall_image_;
    }
}

