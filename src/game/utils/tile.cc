

#include "tile.h"
#include "../../framework/vector2D.h"

namespace utils {

using namespace framework;

void Tile::CleanVisibility(GameMap& map) {

    for (int i = 0; i < (int)map.size(); ++i)
        for (int j = 0; j < (int)map[i].size(); ++j) {
            Tile *tile = map[i][j];
            tile->visible_ = false;
            tile->Uncheck();
        }

}


TilePos Tile::ToTilePos(Vector2D pos) {
    return TilePos(
        static_cast<int>(pos.y + 0.5f),
        static_cast<int>(pos.x + 0.5f)
    );
}

Vector2D Tile::FromTilePos(TilePos pos) {
    return Vector2D(pos.j, pos.i);
}

Tile* Tile::GetFromWorldPosition(GameMap &map, framework::Vector2D pos) {
   TilePos map_pos = ToTilePos(pos);
   map_pos.i = map.size() - map_pos.i - 1;
   return GetFromMapPosition(map, map_pos);
}

}
