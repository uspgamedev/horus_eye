#include <functional>
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
using std::tr1::bind;
using namespace std::tr1::placeholders;

static void On_die_callback(WorldObject* wobj, VirtualObj vobj) {
    vobj();
}

/** arguments[0] is the script name. */
WorldObject* Script(const std::vector<std::string>& arguments) {
    VirtualObj script_generator = SCRIPT_MANAGER()->LoadModule(arguments[0]);
    if(!script_generator["generate"]) return NULL;

    VirtualObj script_data = script_generator["generate"]();
	WorldObject* wobj = new WorldObject;

    if(script_data["drawable"])
        wobj->graphic()->node()->set_drawable(script_data["drawable"].value<ugdk::graphic::Drawable*>(true));

    if(script_data["timed_life"])
        wobj->set_timed_life(script_data["timed_life"].value<double>());

    if(script_data["on_die_callback"])
        wobj->set_die_callback(bind(On_die_callback, _1, script_data["on_die_callback"]));

    if(script_data["collision_object"])
        wobj->set_collision_object(script_data["collision_object"].value<pyramidworks::collision::CollisionObject*>(true));

	return wobj;
}

} // namespace ScriptBuilder
} // namespace builder
