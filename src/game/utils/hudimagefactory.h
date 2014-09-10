#ifndef HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_
#define HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_

#include <ugdk/ui.h>

using ugdk::ui::TexturedRectangle;

namespace utils {

class HudImageFactory {
  public:
    HudImageFactory();
    ~HudImageFactory(){}

    TexturedRectangle* LifeBarImage();
    TexturedRectangle* ManaBarImage();
    TexturedRectangle* TotemImage();
    TexturedRectangle* TotemBottomImage();
    TexturedRectangle* BackImage();
    TexturedRectangle* MummyCounterImage();
    TexturedRectangle* FireballIconImage();
    TexturedRectangle* EarthquakeIconImage();
    TexturedRectangle* LightningIconImage();
    TexturedRectangle* EyeImage();
    TexturedRectangle* LightIconImage();
    TexturedRectangle* SandstormIconImage();
    TexturedRectangle* MeteorIconImage();
};

}

#endif // HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_
