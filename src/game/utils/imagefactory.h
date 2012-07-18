#ifndef HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
#define HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_

#include <ugdk/graphic.h>

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
    ugdk::graphic::Spritesheet* ExplosionImage();
    ugdk::graphic::Spritesheet* QuakeImage();
    ugdk::graphic::Spritesheet* ShieldImage();
    ugdk::graphic::Spritesheet* DoorImage();
    ugdk::graphic::Spritesheet* WallImage();
    ugdk::graphic::Spritesheet* EntryImage();
    ugdk::graphic::Spritesheet* LightningImage();
    ugdk::graphic::Spritesheet* LightImage();
    ugdk::graphic::Spritesheet* TileSwitchImage();
    ugdk::graphic::Drawable* LifePotionImage();
    ugdk::graphic::Drawable* ManaPotionImage();
    ugdk::graphic::Drawable* SightPotionImage();
    ugdk::graphic::Drawable* BlueGemImage();
    ugdk::graphic::Drawable* FloorImage();
};

}

#endif // HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
