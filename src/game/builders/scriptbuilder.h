#ifndef HORUSEYE_GAME_BUILDERS_SCRIPTBUILDER_H_
#define HORUSEYE_GAME_BUILDERS_SCRIPTBUILDER_H_

#include <string>
#include <vector>
#include <ugdk/script.h>
#include "game/sprites.h"

namespace builder {
namespace ScriptBuilder {

sprite::WObjPtr Script(const std::string& script_name, const ugdk::script::VirtualObj& params);

} // namespace ScriptBuilder
} // namespace builder

#endif /* HORUSEYE_GAME_BUILDERS_SCRIPTBUILDER_H_ */
