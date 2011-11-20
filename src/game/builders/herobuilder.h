#ifndef HORUS_EYE_GAME_BUILDERS_HERO_BUILDER_H_
#define HORUS_EYE_GAME_BUILDERS_HERO_BUILDER_H_

namespace sprite {
class Hero;
}
namespace utils {
class ImageFactory;
}

namespace builder {

class HeroBuilder {
  public:
    HeroBuilder(utils::ImageFactory *factory) : factory_(factory) {}
    sprite::Hero *Kha();

  private:
    utils::ImageFactory *factory_;
};

} // namespace builder

#endif /* HORUS_EYE_GAME_BUILDERS_HERO_BUILDER_H_ */
