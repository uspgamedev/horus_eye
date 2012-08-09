
#ifndef HORUSEYE_CONTEXT_H_
#define HORUSEYE_CONTEXT_H_

#include <string>

#include "game/sprites/worldobject.h"

namespace context {

sprite::WorldObject* WorldObjectByTag (const std::string& tag);

void CreateTag (sprite::WorldObject* obj, const std::string& tag);

} // namespace context


#endif /* HORUSEYE_CONTEXT_H_ */
