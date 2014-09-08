#ifndef HORUSEYE_GAME_UTILS_TILEFWD_H_
#define HORUSEYE_GAME_UTILS_TILEFWD_H_

#include <vector>

namespace map {

class Room;
class GiantFloor;
class Tile;
typedef std::vector<Tile*> TileRow;
typedef std::vector<TileRow> GameMap;

}

#endif
