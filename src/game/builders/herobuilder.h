#ifndef HORUS_EYE_GAME_BUILDERS_HERO_BUILDER_H_
#define HORUS_EYE_GAME_BUILDERS_HERO_BUILDER_H_

#include "game/sprites.h"

namespace utils {
class ImageFactory;
}

namespace builder {
class HeroBuilder {
  public:
    HeroBuilder(utils::ImageFactory *factory) : factory_(factory) {}
    sprite::WorldObject *Kha();

    void SetupCollision(sprite::WorldObject* obj);

  private:
    utils::ImageFactory *factory_;
};

} // namespace builder

#endif /* HORUS_EYE_GAME_BUILDERS_HERO_BUILDER_H_ */
