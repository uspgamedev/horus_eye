#ifndef HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_
#define HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_

#include <ugdk/graphic.h>
#include <ugdk/ui.h>
#include <memory>

namespace utils {

class MenuImageFactory {
  public:
    MenuImageFactory();
    ~MenuImageFactory(){}

    std::pair<
        ugdk::ui::Drawable*,
        std::shared_ptr<ugdk::graphic::SpriteAnimationPlayer>
    > HorusEye();
};

}

#endif // HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_
