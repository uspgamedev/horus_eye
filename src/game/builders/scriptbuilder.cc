#include <functional>
#include <ugdk/graphic/node.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>
#include <pyramidworks/collision/collisionobject.h>

#include "scriptbuilder.h"

#include "game/components/graphic.h"
#include "game/components/logic.h"
#include "game/sprites/worldobject.h"
#include "game/scenes/world.h"

namespace builder {
namespace ScriptBuilder {
    
using ugdk::script::VirtualObj;
using sprite::WorldObject;
using std::tr1::bind;
using pyramidworks::collision::CollisionObject;
using namespace std::tr1::placeholders;

static void On_die_callback(WorldObject* wobj, VirtualObj vobj) {
    vobj();
}


static void create_drawable(WorldObject* wobj, VirtualObj data) {
    wobj->graphic()->node()->set_drawable(data.value<ugdk::graphic::Drawable*>(true));
}
static void create_timedlife(WorldObject* wobj, VirtualObj data) {
    wobj->set_timed_life(data.value<double>());
}
static void create_die_callback(WorldObject* wobj, VirtualObj data) {
    wobj->set_die_callback(bind(On_die_callback, _1, data));
}
static void create_collision(WorldObject* wobj, VirtualObj coldata) {
    CollisionObject* colobj = new CollisionObject(WORLD()->collision_manager(), wobj);
    wobj->set_collision_object(colobj);

    colobj->InitializeCollisionClass(coldata["class"].value<std::string>());
    colobj->set_shape(coldata["shape"].value<pyramidworks::geometry::GeometricShape*>(true));
}

typedef void (*ScriptWobj)(WorldObject*, VirtualObj);
struct {
    std::string name;
    ScriptWobj func;
} valid_names[] = {
    { "drawable", create_drawable },
    { "timed_life", create_timedlife },
    { "on_die_callback", create_die_callback },
    { "collision", create_collision }
};
#define NUM_FIELDS 4


/** arguments[0] is the script name. */
WorldObject* Script(const std::vector<std::string>& arguments) {
    VirtualObj script_generator = SCRIPT_MANAGER()->LoadModule(arguments[0]);
    if(!script_generator["generate"]) return NULL;


    VirtualObj script_data = script_generator["generate"]();
    WorldObject* wobj = new WorldObject;

    for(int i = 0; i < NUM_FIELDS; ++i) {
        VirtualObj data = script_data[valid_names[i].name];
        if(data) valid_names[i].func(wobj, data);
    }

    return wobj;
}

} // namespace ScriptBuilder
} // namespace builder
