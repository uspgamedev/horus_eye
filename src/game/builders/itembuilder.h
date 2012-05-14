#ifndef HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_

#include <ugdk/graphic.h>

namespace sprite {
class WorldObject;
}

namespace builder {

class ItemBuilder {
  public:
    ItemBuilder() {}
    ~ItemBuilder() {}

    sprite::WorldObject* LifePotion(ugdk::graphic::Drawable* image);
    sprite::WorldObject* ManaPotion(ugdk::graphic::Drawable* image);
    sprite::WorldObject* SightPotion(ugdk::graphic::Drawable* image);
    sprite::WorldObject* BlueGem(ugdk::graphic::Drawable* image);
};

}

#endif /* HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_ */
