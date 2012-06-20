#ifndef HORUSEYE_GAME_BUILDERS_DOODADBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_DOODADBUILDER_H_

#include <vector>
#include "game/scenes.h"
#include "game/sprites.h"

namespace builder {
namespace DoodadBuilder {

sprite::WorldObject* Door(const std::vector<std::string>& arguments, scene::World* world);
sprite::WorldObject* Wall(const std::vector<std::string>& arguments);
sprite::WorldObject* Entry(const std::vector<std::string>& arguments);
sprite::WorldObject* Button(const std::vector<std::string>& arguments);
sprite::WorldObject* Block(const std::vector<std::string>& arguments);

} // namespace DoodadBuilder
} // namespace builder

#endif /* HORUSEYE_GAME_BUILDERS_DOODADBUILDER_H_ */
