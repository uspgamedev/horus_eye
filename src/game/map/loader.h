#ifndef HORUSEYE_GAME_MAP_LOADER_H_
#define HORUSEYE_GAME_MAP_LOADER_H

#include <string>
#include <ugdk/math/integer2D.h>
#include "game/map.h"
#include <ugdk/script.h>

namespace map {

Room* LoadRoom(const std::string& name, const std::string& campaign, const ugdk::math::Integer2D& position);
Room* LoadRoom(const std::string& name, const ugdk::script::VirtualObj& room_script, const ugdk::math::Integer2D& position);

}

#endif // HORUSEYE_GAME_MAP_LOADER_H
