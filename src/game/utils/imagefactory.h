//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/imagefactory.h
// Definicao da classe ImageFactory.
//

#ifndef HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
#define HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_

#include "../../framework/image.h"
using namespace framework;
namespace utils {

class ImageFactory {
  public:
    ImageFactory();
    virtual ~ImageFactory(){}
    virtual Image* HeroImage();
    virtual Image* MummyImage();
    virtual Image* RangedMummyImage();
    virtual Image* BigMummyImage();
    virtual Image* LifeImage();

  protected:
    Image* hero_image_;
    Image* mummy_image_;
    Image* ranged_mummy_image_;
    Image* big_mummy_image_;
    Image* life_image_;

};

}

#endif // HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
