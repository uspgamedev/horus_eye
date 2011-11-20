#ifndef HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
#define HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_

namespace ugdk {
class Image;
}
using ugdk::Image;
namespace utils {

class ImageFactory {
  public:
    ImageFactory() {}
    virtual ~ImageFactory(){}

    virtual Image* HeroImage();
    virtual Image* MummyImage();
    virtual Image* RangedMummyImage();
    virtual Image* BigMummyImage();
    virtual Image* PharaohImage();
    virtual Image* MagicMissileImage();
    virtual Image* FireballImage();
    virtual Image* MummyProjectileImage();
    virtual Image* ExplosionImage();
    virtual Image* QuakeImage();
    virtual Image* LifePotionImage();
    virtual Image* ManaPotionImage();
    virtual Image* SightPotionImage();
    virtual Image* DoorImage();
    virtual Image* FloorImage();
    virtual Image* WallImage();
    virtual Image* EntryImage();
    virtual Image* LightningImage();
	virtual Image* LightImage();
};

}

#endif // HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
