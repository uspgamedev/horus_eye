#ifndef HORUS_EYE_GAME_BUILDERS_BUILDER_H_
#define HORUS_EYE_GAME_BUILDERS_BUILDER_H_

#include <vector>
#include <string>
#include "game/sprites.h"

namespace builder {

typedef std::vector<std::string> ArgumentList;

sprite::WObjPtr WorldObjectFromTypename(const std::string& type, const ArgumentList& arguments);

bool HasFactoryMethod(const std::string& type);

} // namespace builder

#endif /* HORUS_EYE_GAME_BUILDERS_BUILDER_H_ */
