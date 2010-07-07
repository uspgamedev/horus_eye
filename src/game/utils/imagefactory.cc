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
        hero_image_ = VIDEO_MANAGER()->LoadImage("data/images/mage_74x74.png");
        mummy_image_ = VIDEO_MANAGER()->LoadImage("data/images/mage_red_74x74.png");
    }
    Image* ImageFactory::HeroImage(){
        return hero_image_;
    }
    Image* ImageFactory::MummyImage(){
        return mummy_image_;
    }
}

