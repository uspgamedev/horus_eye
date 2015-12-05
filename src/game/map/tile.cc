#include "tile.h"

#include <ugdk/math/vector2D.h>
#include <ugdk/math/geometry.h>

namespace map {

Tile::Tile (int i, int j, char object)
 : pos_(i,j)
 , object_(object)
{}


TilePos Tile::ToTilePos(ugdk::math::Vector2D pos) {
    return TilePos(
        static_cast<int>(pos.y + 0.5),
        static_cast<int>(pos.x + 0.5)
    );
}

ugdk::math::Vector2D Tile::FromTilePos(TilePos pos) {
    return ugdk::math::Vector2D(pos.j, pos.i);
}

Tile* Tile::GetFromWorldPosition(GameMap &map, ugdk::math::Vector2D pos) {
   TilePos map_pos = ToTilePos(pos);
   map_pos.i = map.size() - map_pos.i - 1;
   return GetFromMapPosition(map, map_pos);
}

}
