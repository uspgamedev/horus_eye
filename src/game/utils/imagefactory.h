#ifndef HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
#define HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_

#include <ugdk/graphic/spritesheet/flexiblespritesheet.h>

using ugdk::graphic::FlexibleSpritesheet;
namespace utils {

class ImageFactory {
  public:
    ImageFactory() {}
    ~ImageFactory(){}

    ugdk::graphic::Spritesheet* HeroImage();
    FlexibleSpritesheet* MummyImage();
    FlexibleSpritesheet* RangedMummyImage();
    FlexibleSpritesheet* BigMummyImage();
    FlexibleSpritesheet* PharaohImage();
    FlexibleSpritesheet* MagicMissileImage();
    FlexibleSpritesheet* FireballImage();
    FlexibleSpritesheet* MummyProjectileImage();
    ugdk::graphic::Spritesheet* ExplosionImage();
    FlexibleSpritesheet* QuakeImage();
    ugdk::graphic::Drawable* LifePotionImage();
    ugdk::graphic::Drawable* ManaPotionImage();
    ugdk::graphic::Drawable* SightPotionImage();
    ugdk::graphic::Drawable* BlueGemImage();
    ugdk::graphic::Spritesheet* ShieldImage();
    ugdk::graphic::Spritesheet* DoorImage();
    ugdk::graphic::Drawable* FloorImage();
    ugdk::graphic::Spritesheet* WallImage();
    ugdk::graphic::Spritesheet* EntryImage();
    FlexibleSpritesheet* LightningImage();
    FlexibleSpritesheet* LightImage();
    ugdk::graphic::Spritesheet* TileSwitchImage();
};

}

#endif // HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
