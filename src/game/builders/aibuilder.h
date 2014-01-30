#ifndef HORUS_EYE_GAME_BUILDERS_AI_BUILDER_H_
#define HORUS_EYE_GAME_BUILDERS_AI_BUILDER_H_

#include <vector>
#include <string>
#include "game/ai.h"
#include "game/sprites.h"

namespace builder {
namespace AIBuilder {

ai::AI* AIScript(const std::string& script_name);
ai::AI* AIScript(const std::vector<std::string>& arguments);

}
}
#endif /* HORUS_EYE_GAME_BUILDERS_AI_BUILDER_H_ */
