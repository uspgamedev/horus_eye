#ifndef HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_

namespace ugdk {
class FlexibleSpritesheet;
}

namespace sprite {
class Item;
}

namespace builder {

class ItemBuilder {
  public:
    ItemBuilder() {}
    ~ItemBuilder() {}

    sprite::Item* LifePotion(ugdk::FlexibleSpritesheet* image);
    sprite::Item* ManaPotion(ugdk::FlexibleSpritesheet* image);
    sprite::Item* SightPotion(ugdk::FlexibleSpritesheet* image);
};

}

#endif /* HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_ */
