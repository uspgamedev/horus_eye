#ifndef HORUSEYE_GAME_SPRITE_FLOOR_H_
#define HORUSEYE_GAME_SPRITE_FLOOR_H_

#include "game/sprites/worldobject.h"

namespace ugdk {
	class Image;
    class Sprite;
};
namespace utils {
    class Tile;
};

namespace sprite {

class Floor : public WorldObject {
  public:
    Floor(ugdk::FlexibleSpritesheet* image);
    virtual void Update(float delta_t);

  protected:
    utils::Tile *tile_;
    ugdk::Sprite *sprite_;
};

}

#endif /* HORUSEYE_GAME_SPRITE_FLOOR_H_ */
