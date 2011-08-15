#ifndef HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
#define HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_

namespace framework {
class Image;
}
using framework::Image;
namespace utils {

class ImageFactory {
  public:
    ImageFactory();
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

  protected:
    Image* hero_image_;
    Image* mummy_image_;
    Image* ranged_mummy_image_;
    Image* big_mummy_image_;
    Image* pharaoh_image_;
    Image* projectile_image_;
    Image* magicmissile_image_;
    Image* mummy_projectile_image_;
    Image* fireball_image_;
    Image* explosion_image_;
    Image* quake_image_;
    Image* life_potion_image_;
    Image* mana_potion_image_;
    Image* sight_potion_image_;
    Image* door_image_;
    Image* floor_image_;
    Image* wall_image_;
    Image* entry_image_;
    Image* lightning_image_;
};

}

#endif // HORUSEYE_GAME_UTILS_IMAGEFACTORY_H_
