#ifndef HORUSEYE_GAME_SPRITE_FLOOR_H_
#define HORUSEYE_GAME_SPRITE_FLOOR_H_

#include "game/sprites/worldobject.h"

namespace ugdk {
	class Image;
};
namespace utils {
    class Tile;
};

namespace sprite {

class Floor : public WorldObject {
  public:
    Floor(ugdk::Image* image);
    virtual void Update(float delta_t);

  private:
    utils::Tile *tile_;
};

}

#endif /* HORUSEYE_GAME_SPRITE_FLOOR_H_ */
