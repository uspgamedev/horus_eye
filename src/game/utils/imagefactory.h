//
// Horus Eye
// Copyright (C) 2010  USPGameDev
//
// game/utils/imagefactory.h
// Definicao da classe ImageFactory.
//

#ifndef ORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
#define ORUSEYE_GAME_UTILS_IMAGEFACTORY_H_

#include "../../framework/image.h"
using namespace framework;
namespace utils {

class ImageFactory {
  public:
    ImageFactory();
    virtual ~ImageFactory(){}
    virtual Image* HeroImage();
    virtual Image* MummyImage();

  protected:
    Image* hero_image_;
    Image* mummy_image_;

};

}

#endif // HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
