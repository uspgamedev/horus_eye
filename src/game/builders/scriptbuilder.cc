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

static CollisionObject* create_collision(WorldObject* wobj, VirtualObj coldata) {
    if(!check_for_fields(coldata, "class", "shape")) {
        fprintf(stdout, "Warning: collision description without required fields.");
        return NULL;
    }

    using pyramidworks::geometry::GeometricShape;
    GeometricShape* shape = coldata["shape"].value<GeometricShape*>(true);
    if(!shape) {
        fprintf(stdout, "Warning: field 'shape' has invalid value.");
        return NULL;
    }

    CollisionObject* colobj = new CollisionObject(WORLD()->collision_manager(), wobj);

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

    return colobj;
}

static void post_build(WorldObject* wobj, const VirtualObj& descriptor) {
    CollisionObject *collision = NULL, *visibility = NULL;

    if(descriptor["on_die_callback"]) {
        VirtualObj callback_function = descriptor["on_die_callback"];
        wobj->set_die_callback([callback_function](WorldObject* obj) -> void {
            VirtualObj arg = VirtualObj(callback_function.wrapper());
            arg.set_value<WorldObject*>(obj);
            callback_function(VirtualObj::List(1, arg));
        });
    }

    if(descriptor["collision"])
        collision = create_collision(wobj, descriptor["collision"]);

    if(descriptor["visibility"])
        visibility = create_collision(wobj, descriptor["visibility"]);

    if(collision || visibility)
        wobj->AddComponent(new component::Shape(collision, visibility));
}

/** arguments[0] is the script name. */
WorldObject* Script(const vector<string>& arguments) {
    if (arguments.empty()) return NULL;
    VirtualObj script_generator = SCRIPT_MANAGER()->LoadModule("objects." + arguments[0]);
    if(!script_generator) {
        fprintf(stderr, "Unable to load 'objects.%s'.\n", arguments[0].c_str());
        return NULL;
    }
    if(!script_generator["build"]) {
        fprintf(stderr, "Function 'build' not found in 'objects.%s'.\n", arguments[0].c_str());
        return NULL;
    }
    
    WorldObject* wobj = new WorldObject;
    wobj->set_identifier(arguments[0]);

    VirtualObj v_wobj(script_generator["build"].wrapper());
    v_wobj.set_value<WorldObject*>(wobj);
    
    VirtualObj::List args(1, v_wobj);

    for (vector<string>::const_iterator it = arguments.begin()+1; it != arguments.end(); it++) {
        VirtualObj obj(script_generator["build"].wrapper());
        obj.set_value<string>(*it);
        args.push_back(obj);
    }

    VirtualObj build_result = script_generator["build"](args);
    if(build_result) {
        post_build(wobj, build_result);
    }

    return wobj;
}

sprite::WorldObject* Script(const std::string& script_name) {
    return Script(vector<string>(1, script_name));
}

} // namespace ScriptBuilder
} // namespace builder
