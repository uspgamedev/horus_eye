//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/imagefactory.h
// Definicao da classe ImageFactory.
//

#include"imagefactory.h"
#include"../scenes/world.h"
namespace utils {
    ImageFactory::ImageFactory(){
        hero_image_ = VIDEO_MANAGER()->LoadImage("data/images/sprite-sheet_MOD3.png");
        mummy_image_ = VIDEO_MANAGER()->LoadImage("data/images/sprite-sheet_MOD3_bw.png");
        life_image_  = VIDEO_MANAGER()->LoadImage("data/images/eye_animation.png");
    }
    Image* ImageFactory::HeroImage(){
        return hero_image_;
    }
    Image* ImageFactory::MummyImage(){
        return mummy_image_;
    }
    Image* ImageFactory::LifeImage() {
        return life_image_;
    }
}

