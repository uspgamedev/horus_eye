#ifndef HORUSEYE_GAME_UTILS_LEVELLOADER_H_
#define HORUSEYE_GAME_UTILS_LEVELLOADER_H_

#include "game/scenes.h"
#include <ugdk/script.h>
#include <string>

namespace utils {

void LoadLevel(const ugdk::script::VirtualObj& level_data, const std::string& level_path, scene::World** world);

}

#endif // HORUSEYE_GAME_UTILS_LEVELLOADER_H_
