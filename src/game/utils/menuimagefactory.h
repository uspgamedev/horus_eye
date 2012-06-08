#ifndef HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_
#define HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_

#include <ugdk/graphic.h>

namespace utils {

class MenuImageFactory {
  public:
    MenuImageFactory();
    ~MenuImageFactory(){}

    ugdk::graphic::Sprite* HorusEye();
};

}

#endif // HORUSEYE_GAME_UTILS_HUDIMAGEFACTORY_H_
