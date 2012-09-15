#ifndef HORUSEYE_GAME_BUILDERS_DOODADBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_DOODADBUILDER_H_

#include <vector>
#include "game/sprites.h"

namespace builder {
namespace DoodadBuilder {

sprite::WorldObject* Door(const std::vector<std::string>& arguments);
sprite::WorldObject* Wall(const std::vector<std::string>& arguments);
sprite::WorldObject* Entry(const std::vector<std::string>& arguments);
sprite::WorldObject* Button(const std::vector<std::string>& arguments);
sprite::WorldObject* Block(const std::vector<std::string>& arguments);

} // namespace DoodadBuilder
} // namespace builder

#endif /* HORUSEYE_GAME_BUILDERS_DOODADBUILDER_H_ */
