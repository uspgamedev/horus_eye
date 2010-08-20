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

        life_image_  = VIDEO_MANAGER()->LoadImage("data/images/eye_animation.png");
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
    Image* ImageFactory::LifeImage() {
        return life_image_;
    }
}

