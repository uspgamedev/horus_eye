#include <functional>
#include <ugdk/graphic/node.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/genericcollisionlogic.h>

#include "scriptbuilder.h"

#include "game/components/graphic.h"
#include "game/components/damageable.h"
#include "game/components/shape.h"
#include "game/sprites/worldobject.h"
#include "game/scenes/world.h"

namespace builder {
namespace ScriptBuilder {
    
using ugdk::script::VirtualObj;
using sprite::WorldObject;
using std::tr1::bind;
using std::string;
using std::vector;
using pyramidworks::collision::CollisionObject;
using pyramidworks::collision::CollisionLogic;
using pyramidworks::collision::GenericCollisionLogic;
using namespace std::tr1::placeholders;

bool check_for_fields(const VirtualObj& logic, const std::string& f1 = std::string(), const std::string& f2 = std::string(),
    const std::string& f3 = std::string(), const std::string& f4 = std::string(), const std::string& f5 = std::string()) {

    if(!logic) return false;
    if(!f1.empty() && !logic[f1]) return false;
    if(!f2.empty() && !logic[f2]) return false;
    if(!f3.empty() && !logic[f3]) return false;
    if(!f4.empty() && !logic[f4]) return false;
    if(!f5.empty() && !logic[f5]) return false;
    return true;
}

class ScriptCollision {
  public:
    ScriptCollision (const VirtualObj& logic, WorldObject* obj) :
        logic_(logic), self_(obj) {}
    void operator () (void* data) {
        WorldObject *another = static_cast<WorldObject*>(data);
        VirtualObj  self (logic_.wrapper()),
                    target (logic_.wrapper());
        self.set_value<WorldObject*>(self_);
        target.set_value<WorldObject*>(another);
        logic_((self, target));
    }
  private:
    VirtualObj  logic_;
    WorldObject *self_;
};

static void create_collision(WorldObject* wobj, VirtualObj coldata) {
    if(!check_for_fields(coldata, "class", "shape")) {
        fprintf(stdout, "Warning: collision description without required fields.");
        return;
    }

    using pyramidworks::geometry::GeometricShape;
    GeometricShape* shape = coldata["shape"].value<GeometricShape*>(true);
    if(!shape) {
        fprintf(stdout, "Warning: field 'shape' has invalid value.");
        return;
    }

    CollisionObject* colobj = new CollisionObject(WORLD()->collision_manager(), wobj);
    wobj->AddComponent(new component::Shape(colobj, NULL));

    colobj->InitializeCollisionClass(coldata["class"].value<std::string>());
    colobj->set_shape(shape);

    if(coldata["known_collision"]) {
        VirtualObj::Map custom_collisions = coldata["known_collision"].value<VirtualObj::Map>();
        for(VirtualObj::Map::iterator it = custom_collisions.begin(); it != custom_collisions.end(); ++it) {
            std::string classname = it->first.value<std::string>();
            VirtualObj scriptlogic = it->second;
            CollisionLogic* logic = new GenericCollisionLogic(ScriptCollision(scriptlogic, wobj));
            colobj->AddCollisionLogic(classname, logic);
        }
    }
}

static void create_drawable(WorldObject* wobj, VirtualObj data) {
    wobj->AddComponent(new component::BaseGraphic(data.value<ugdk::graphic::Drawable*>(true)));
}
static void create_timedlife(WorldObject* wobj, VirtualObj data) {
    wobj->set_timed_life(data.value<double>());
}

static void On_die_callback(WorldObject* wobj, VirtualObj vobj) {
    VirtualObj arg = VirtualObj(vobj.wrapper());
    arg.set_value<WorldObject*>(wobj);
    vobj(VirtualObj::List(1, arg));
}
static void create_die_callback(WorldObject* wobj, VirtualObj data) {
    wobj->set_die_callback(bind(On_die_callback, _1, data));
}
static void create_light_radius(WorldObject* wobj, VirtualObj data) {
    wobj->graphic()->ChangeLightRadius(data.value<double>());
}

#define NUM_FIELDS 5
typedef void (*ScriptWobj)(WorldObject*, VirtualObj);
struct ValidNameStruct { // Compiler doesn't like an annonymous struct here
    std::string name;
    ScriptWobj func;
} valid_names[NUM_FIELDS] = {
    {        "drawable", create_drawable     },
    {      "timed_life", create_timedlife    },
    { "on_die_callback", create_die_callback },
    {    "light_radius", create_light_radius },
    {       "collision", create_collision    }
};


/** arguments[0] is the script name. */
WorldObject* Script(const vector<string>& arguments) {
    if (arguments.empty()) return NULL;
    VirtualObj script_generator = SCRIPT_MANAGER()->LoadModule("objects." + arguments[0]);
    if(!script_generator) {
        fprintf(stderr, "Unable to load 'objects.%s'.\n", arguments[0].c_str());
        return NULL;
    }
    if(!script_generator["generate"]) {
        fprintf(stderr, "Function 'generate' not found in 'objects.%s'.\n", arguments[0].c_str());
        return NULL;
    }

    VirtualObj::List args;
    for (vector<string>::const_iterator it = arguments.begin()+1; it != arguments.end(); it++) {
        VirtualObj obj(script_generator.wrapper());
        obj.set_value<string>(*it);
        args.push_back(obj);
    }
    VirtualObj script_data = script_generator["generate"](args);
    if(!script_data) {
        fprintf(stderr, "Function 'generate' didn't return a valid object in 'objects.%s'.\n", arguments[0].c_str());
        return NULL;
    }

    WorldObject* wobj = new WorldObject;
    wobj->set_identifier(arguments[0]);

    for(int i = 0; i < NUM_FIELDS; ++i) {
        VirtualObj data = script_data[valid_names[i].name];
        if(data) valid_names[i].func(wobj, data);
    }

    VirtualObj script_builder = script_generator["build"];
    if (script_builder) {
        VirtualObj::List    args;
        VirtualObj          v_wobj(script_builder.wrapper());
        v_wobj.set_value<WorldObject*>(wobj);
        args.push_back(v_wobj);
        script_builder(args);
    }

    return wobj;
}

sprite::WorldObject* Script(const std::string& script_name) {
    return Script(vector<string>(1, script_name));
}

} // namespace ScriptBuilder
} // namespace builder
