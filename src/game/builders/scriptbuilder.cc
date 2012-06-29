#include "scriptbuilder.h"

#include "game/sprites/worldobject.h"

namespace builder {
namespace ScriptBuilder {

using sprite::WorldObject;

WorldObject* Script(const std::vector<std::string>& arguments) {
	WorldObject* wobj = new WorldObject;
	
	return wobj;
}

} // namespace ScriptBuilder
} // namespace builder
