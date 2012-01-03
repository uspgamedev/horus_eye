#ifndef HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
#define HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_

#include <ugdk/graphic/drawable/flexiblespritesheet.h>

using ugdk::FlexibleSpritesheet;
namespace utils {

class ImageFactory {
  public:
    ImageFactory() {}
    virtual ~ImageFactory(){}

    virtual FlexibleSpritesheet* HeroImage();
    virtual FlexibleSpritesheet* MummyImage();
    virtual FlexibleSpritesheet* RangedMummyImage();
    virtual FlexibleSpritesheet* BigMummyImage();
    virtual FlexibleSpritesheet* PharaohImage();
    virtual FlexibleSpritesheet* MagicMissileImage();
    virtual FlexibleSpritesheet* FireballImage();
    virtual FlexibleSpritesheet* MummyProjectileImage();
    virtual FlexibleSpritesheet* ExplosionImage();
    virtual FlexibleSpritesheet* QuakeImage();
    virtual FlexibleSpritesheet* LifePotionImage();
    virtual FlexibleSpritesheet* ManaPotionImage();
    virtual FlexibleSpritesheet* SightPotionImage();
    virtual FlexibleSpritesheet* DoorImage();
    virtual FlexibleSpritesheet* FloorImage();
    virtual FlexibleSpritesheet* WallImage();
    virtual FlexibleSpritesheet* EntryImage();
    virtual FlexibleSpritesheet* LightningImage();
	virtual FlexibleSpritesheet* LightImage();
    virtual FlexibleSpritesheet* TileSwitchImage();
};

}

#endif // HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
