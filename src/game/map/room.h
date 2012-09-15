#ifndef HORUSEYE_GAME_MAP_ROOM_H_
#define HORUSEYE_GAME_MAP_ROOM_H_

#include <list>
#include <vector>
#include "game/sprites.h"
#include "game/map.h"

namespace map {

class Room {
  public:
    Room() {}
    ~Room() {}

private:
    std::vector<sprite::WorldObject*> walls_;
    std::list<sprite::WorldObject*> objects_;
    GameMap level_matrix_;
};

} // namespace map

#endif /* HORUSEYE_GAME_MAP_ROOM_H_ */
