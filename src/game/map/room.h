#ifndef HORUSEYE_GAME_MAP_ROOM_H_
#define HORUSEYE_GAME_MAP_ROOM_H_

#include <list>
#include <vector>
#include <ugdk/graphic.h>
#include <ugdk/math/vector2D.h>
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
    ugdk::graphic::Node* floor_;
    GameMap level_matrix_;
    ugdk::Vector2D offset_;
};

} // namespace map

#endif /* HORUSEYE_GAME_MAP_ROOM_H_ */
