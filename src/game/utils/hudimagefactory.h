#ifndef HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_
#define HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_

namespace ugdk {
class Image;
}
using ugdk::Image;
namespace utils {

class HudImageFactory {
  public:
    HudImageFactory();
    virtual ~HudImageFactory(){}

    virtual Image* LifeBarImage();
    virtual Image* ManaBarImage();
    virtual Image* TotemImage();
    virtual Image* TotemBottomImage();
    virtual Image* BackImage();
    virtual Image* MummyCounterImage();
    virtual Image* FireballIconImage();
    virtual Image* EarthquakeIconImage();
    virtual Image* LightningIconImage();
    virtual Image* EyeImage();

  protected:
    Image* life_bar_image_;
    Image* mana_bar_image_;
    Image* totem_image_;
    Image* totem_bottom_image_;
    Image* back_image_;
    Image* mummy_counter_image_;
    Image* fireball_icon_image_;
    Image* earthquake_icon_image_;
    Image* lightning_icon_image_;
    Image* eye_image_;
};

}

#endif // HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_
