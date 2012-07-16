#ifndef HORUSEYE_GAME_BUILDERS_SCRIPTBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_SCRIPTBUILDER_H_

#include <vector>
#include "game/sprites.h"

namespace builder {
namespace ScriptBuilder {

sprite::WorldObject* Script(const std::vector<std::string>& arguments);

} // namespace ScriptBuilder
} // namespace builder

#endif /* HORUSEYE_GAME_BUILDERS_SCRIPTBUILDER_H_ */
