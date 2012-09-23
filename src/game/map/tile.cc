#include "tile.h"

#include <ugdk/math/vector2D.h>
#include <ugdk/graphic/node.h>
#include <ugdk/graphic/modifier.h>

namespace map {

Tile::Tile (int i, int j, char object)
    : pos_(i,j), object_(object), visible_(false), floor_(new ugdk::graphic::Node) {
        floor_->modifier()->ToggleFlag(ugdk::graphic::Modifier::TRUNCATES_WHEN_APPLIED);
}

void Tile::CleanVisibility(GameMap& map) {

    for (int i = 0; i < (int)map.size(); ++i)
        for (int j = 0; j < (int)map[i].size(); ++j) {
            Tile *tile = map[i][j];
            tile->visible_ = false;
            tile->floor()->modifier()->set_color(ugdk::Color(0.5, 0.5, 0.5));
            tile->Uncheck();
        }

}


TilePos Tile::ToTilePos(ugdk::Vector2D pos) {
    return TilePos(
        static_cast<int>(pos.y + 0.5),
        static_cast<int>(pos.x + 0.5)
    );
}

ugdk::Vector2D Tile::FromTilePos(TilePos pos) {
    return ugdk::Vector2D(pos.j, pos.i);
}

Tile* Tile::GetFromWorldPosition(GameMap &map, ugdk::Vector2D pos) {
   TilePos map_pos = ToTilePos(pos);
   map_pos.i = map.size() - map_pos.i - 1;
   return GetFromMapPosition(map, map_pos);
}

}
