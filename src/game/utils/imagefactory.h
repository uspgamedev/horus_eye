#ifndef HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
#define HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_

#include <ugdk/graphic.h>
#include <ugdk/internal.h>

namespace utils {

class ImageFactory {
  public:
    ImageFactory() {}
    ~ImageFactory(){}

    ugdk::graphic::TextureAtlas* HeroImage();
    ugdk::graphic::TextureAtlas* MummyImage();
    ugdk::graphic::TextureAtlas* RangedMummyImage();
    ugdk::graphic::TextureAtlas* BigMummyImage();
    ugdk::graphic::TextureAtlas* PharaohImage();
    ugdk::graphic::TextureAtlas* MagicMissileImage();
    ugdk::graphic::TextureAtlas* FireballImage();
    ugdk::graphic::TextureAtlas* MummyProjectileImage();
    ugdk::graphic::TextureAtlas* DoorImage();
    ugdk::graphic::TextureAtlas* WallImage();
    ugdk::graphic::TextureAtlas* EntryImage();
    ugdk::graphic::TextureAtlas* LightningImage();
    ugdk::graphic::TextureAtlas* LightImage();
    ugdk::graphic::TextureAtlas* TileSwitchImage();
    ugdk::internal::GLTexture* LifePotionImage();
    ugdk::internal::GLTexture* ManaPotionImage();
};

}

#endif // HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
