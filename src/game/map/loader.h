#ifndef HORUSEYE_GAME_MAP_LOADER_H_
#define HORUSEYE_GAME_MAP_LOADER_H_

#include "game/map.h"

#include <ugdk/math/integer2D.h>
#include <ugdk/script.h>
#include <pyramidworks/collision.h>

#include <string>

namespace map {

bool IsValidRoomData(const ugdk::script::VirtualObj& room_data);
Room* LoadRoom(const std::string& name, const ugdk::script::VirtualObj& room_script,
               const ugdk::math::Integer2D& position);

}

#endif // HORUSEYE_GAME_MAP_LOADER_H_
