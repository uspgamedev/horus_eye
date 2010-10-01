

#include "tile.h"

namespace utils {

void Tile::CleanVisibility(GameMap& map) {

    for (size_t i = 0; i < map.size(); ++i)
        for (size_t j = 0; j < map[i].size(); ++j)
            map[i][j].visible_ = false;

}

}
