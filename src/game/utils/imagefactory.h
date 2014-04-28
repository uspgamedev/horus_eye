#ifndef HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
#define HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_

#include <ugdk/graphic.h>
#include <ugdk/internal.h>

namespace utils {

class ImageFactory {
  public:
    ImageFactory() {}
    ~ImageFactory(){}

    ugdk::graphic::Spritesheet* HeroImage();
    ugdk::graphic::Spritesheet* MummyImage();
    ugdk::graphic::Spritesheet* RangedMummyImage();
    ugdk::graphic::Spritesheet* BigMummyImage();
    ugdk::graphic::Spritesheet* PharaohImage();
    ugdk::graphic::Spritesheet* MagicMissileImage();
    ugdk::graphic::Spritesheet* FireballImage();
    ugdk::graphic::Spritesheet* MummyProjectileImage();
    ugdk::graphic::Spritesheet* DoorImage();
    ugdk::graphic::Spritesheet* WallImage();
    ugdk::graphic::Spritesheet* EntryImage();
    ugdk::graphic::Spritesheet* LightningImage();
    ugdk::graphic::Spritesheet* LightImage();
    ugdk::graphic::Spritesheet* TileSwitchImage();
    ugdk::internal::GLTexture* LifePotionImage();
    ugdk::internal::GLTexture* ManaPotionImage();
};

}

#endif // HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
