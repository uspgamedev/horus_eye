#include "menuimagefactory.h"

#include <ugdk/graphic/drawable/sprite.h>

using ugdk::graphic::Sprite;

namespace utils {

MenuImageFactory::MenuImageFactory() {}

ugdk::graphic::Sprite* MenuImageFactory::HorusEye() {
    Sprite* sprite = new Sprite("eye", "animations/menu.gdd");
    sprite->animation_player().Select("SELECTION_EYE");
    sprite->set_hotspot(ugdk::graphic::Drawable::CENTER);
    return sprite;
}
}

