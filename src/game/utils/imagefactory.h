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
    virtual Image* ProjectileImage();
    virtual Image* LifePotionImage();
    virtual Image* ManaPotionImage();
    virtual Image* SightPotionImage();
    virtual Image* DoorImage();
    virtual Image* FloorImage();
    virtual Image* WallImage();
    virtual Image* IntroImage();
    virtual Image* WinImage();
    virtual Image* LoseImage();

  protected:
    Image* hero_image_;
    Image* mummy_image_;
    Image* ranged_mummy_image_;
    Image* big_mummy_image_;
    Image* projectile_image_;
    Image* life_potion_image_;
    Image* mana_potion_image_;
    Image* sight_potion_image_;
    Image* door_image_;
    Image* floor_image_;
    Image* wall_image_;
    Image* intro_image_;
    Image* win_image_;
    Image* lose_image_;

};

}

#endif // HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
