#include <ugdk/graphic/node.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>

#include "scriptbuilder.h"

#include "game/components/graphic.h"
#include "game/components/logic.h"
#include "game/sprites/worldobject.h"

namespace builder {
namespace ScriptBuilder {
    
using ugdk::script::VirtualObj;
using sprite::WorldObject;

/** arguments[0] is the script name. */
WorldObject* Script(const std::vector<std::string>& arguments) {
    VirtualObj script_generator = SCRIPT_MANAGER()->LoadModule(arguments[0]);
    if(!script_generator["generate"]) return NULL;

    VirtualObj script_data = script_generator["generate"]();
	WorldObject* wobj = new WorldObject;

    if(script_data["drawable"])
        wobj->graphic()->node()->set_drawable(script_data["drawable"].value<ugdk::graphic::Drawable*>(true));

	return wobj;
}

} // namespace ScriptBuilder
} // namespace builder
