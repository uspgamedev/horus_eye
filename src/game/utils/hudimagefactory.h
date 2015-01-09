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
    static TexturedRectangle* EyeImage();
};

}

#endif // HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_
