#ifndef HORUSEYE_GAME_UTILS_LEVELLOADER_H_
#define HORUSEYE_GAME_UTILS_LEVELLOADER_H_

#include <string>

namespace scene {
    class World;
}

namespace utils {

void LoadLevel(const std::string& campaign, const std::string& level_name, scene::World** world);

}

#endif // HORUSEYE_GAME_UTILS_LEVELLOADER_H_
