#ifndef HORUSEYE_GAME_MAP_LOADER_H_
#define HORUSEYE_GAME_MAP_LOADER_H

#include <string>
#include <ugdk/math/integer2D.h>
#include "game/map.h"

namespace map {

Room* LoadRoom(const std::string& name, const ugdk::math::Integer2D& position);

}

#endif // HORUSEYE_GAME_MAP_LOADER_H
