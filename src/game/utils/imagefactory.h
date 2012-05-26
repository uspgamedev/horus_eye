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
    FlexibleSpritesheet* ExplosionImage();
    FlexibleSpritesheet* QuakeImage();
    ugdk::graphic::Drawable* LifePotionImage();
    ugdk::graphic::Drawable* ManaPotionImage();
    ugdk::graphic::Drawable* SightPotionImage();
    ugdk::graphic::Drawable* BlueGemImage();
    ugdk::graphic::Spritesheet* ShieldImage();
    FlexibleSpritesheet* DoorImage();
    ugdk::graphic::Drawable* FloorImage();
    FlexibleSpritesheet* WallImage();
    FlexibleSpritesheet* EntryImage();
    FlexibleSpritesheet* LightningImage();
    FlexibleSpritesheet* LightImage();
    FlexibleSpritesheet* TileSwitchImage();
};

}

#endif // HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
