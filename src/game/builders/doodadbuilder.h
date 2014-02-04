#ifndef HORUSEYE_GAME_BUILDERS_DOODADBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_DOODADBUILDER_H_

#include <vector>
#include <ugdk/graphic.h>
#include <ugdk/math.h>
#include "game/sprites.h"

namespace builder {
namespace DoodadBuilder {

sprite::WObjPtr Door(const std::vector<std::string>& arguments);
sprite::WObjPtr Wall(const std::vector<std::string>& arguments);
sprite::WObjPtr InvisibleWall(const std::vector<std::string>& arguments);
sprite::WObjPtr BurntWall(const std::vector<std::string>& arguments);
sprite::WObjPtr Entry(const std::vector<std::string>& arguments);

} // namespace DoodadBuilder
} // namespace builder

#endif /* HORUSEYE_GAME_BUILDERS_DOODADBUILDER_H_ */
