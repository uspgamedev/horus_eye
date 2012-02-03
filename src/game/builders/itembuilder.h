#ifndef HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_

#include <ugdk/graphic.h>

namespace sprite {
class Item;
}

namespace builder {

class ItemBuilder {
  public:
    ItemBuilder() {}
    ~ItemBuilder() {}

    sprite::Item* LifePotion(ugdk::graphic::FlexibleSpritesheet* image);
    sprite::Item* ManaPotion(ugdk::graphic::FlexibleSpritesheet* image);
    sprite::Item* SightPotion(ugdk::graphic::FlexibleSpritesheet* image);
};

}

#endif /* HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_ */
