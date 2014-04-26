#ifndef HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_
#define HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_

#include <ugdk/action/spritetypes.h>
#include <ugdk/graphic.h>
#include <memory>

namespace utils {

class MenuImageFactory {
  public:
    MenuImageFactory();
    ~MenuImageFactory(){}

    std::pair<
        ugdk::graphic::Drawable*,
        std::shared_ptr<ugdk::action::SpriteAnimationPlayer>
    > HorusEye();
};

}

#endif // HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_
