#ifndef HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_
#define HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_

#include <ugdk/ui.h>

using ugdk::ui::TexturedRectangle;

namespace utils {

class HudImageFactory {
  public:
    HudImageFactory();
    ~HudImageFactory(){}

    static TexturedRectangle* LifeBarImage();
    static TexturedRectangle* ManaBarImage();
    static TexturedRectangle* TotemImage();
    static TexturedRectangle* TotemBottomImage();
    static TexturedRectangle* BackImage();
    static TexturedRectangle* MummyCounterImage();
    static TexturedRectangle* FireballIconImage();
    static TexturedRectangle* EarthquakeIconImage();
    static TexturedRectangle* LightningIconImage();
    static TexturedRectangle* EyeImage();
    static TexturedRectangle* LightIconImage();
    static TexturedRectangle* SandstormIconImage();
    static TexturedRectangle* MeteorIconImage();
};

}

#endif // HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_
