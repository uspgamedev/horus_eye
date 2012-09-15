#ifndef HORUSEYE_GAME_MAP_LOADER_H_
#define HORUSEYE_GAME_MAP_LOADER_H

#include <string>
#include "game/map.h"

namespace map {

Room* LoadRoom(const std::string& name);

}

#endif // HORUSEYE_GAME_MAP_LOADER_H
