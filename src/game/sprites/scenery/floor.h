#ifndef HORUSEYE_GAME_SPRITE_FLOOR_H_
#define HORUSEYE_GAME_SPRITE_FLOOR_H_

#include <ugdk/graphic.h>
#include "game/sprites/worldobject.h"

namespace utils {
    class Tile;
};

namespace sprite {

class Floor : public WorldObject {
  typedef WorldObject super;
  public:
    Floor(ugdk::graphic::FlexibleSpritesheet* image);
    virtual void Update(double delta_t);

    virtual void set_world_position(const ugdk::Vector2D& pos);

  protected:
    utils::Tile *tile_;
    ugdk::graphic::Sprite *sprite_;
};

}

#endif /* HORUSEYE_GAME_SPRITE_FLOOR_H_ */
