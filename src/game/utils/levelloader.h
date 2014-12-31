#ifndef HORUSEYE_GAME_UTILS_LEVELLOADER_H_
#define HORUSEYE_GAME_UTILS_LEVELLOADER_H_

#include "game/core.h"
#include <ugdk/script.h>
#include <string>

namespace utils {

core::World* LoadLevel(const ugdk::script::VirtualObj& level_data, const std::string& level_path);

}

#endif // HORUSEYE_GAME_UTILS_LEVELLOADER_H_
