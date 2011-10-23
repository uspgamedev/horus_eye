#ifndef HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_

namespace ugdk {
class Image;
}

namespace sprite {
class Item;
}

namespace builder {

class ItemBuilder {
  public:
    ItemBuilder() {}
    ~ItemBuilder() {}

    sprite::Item* LifePotion(ugdk::Image* image);
    sprite::Item* ManaPotion(ugdk::Image* image);
    sprite::Item* SightPotion(ugdk::Image* image);
};

}

#endif /* HORUSEYE_GAME_BUILDERS_ITEMBUILDER_H_ */
