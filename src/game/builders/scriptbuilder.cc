#include <functional>
#include <ugdk/graphic/node.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>
#include <pyramidworks/collision/collisionobject.h>

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
using std::bind;
using std::string;
using std::vector;
using pyramidworks::collision::CollisionObject;
using pyramidworks::collision::CollisionLogic;
using namespace std::placeholders;

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

CollisionLogic ScriptCollision(VirtualObj logic, WorldObject* owner) {
    return [logic, owner](ugdk::action::Entity* obj) {
        WorldObject *another = dynamic_cast<WorldObject*>(obj);
        VirtualObj  self (logic.wrapper()),
                    target (logic.wrapper());
        self.set_value<WorldObject*>(owner);
        target.set_value<WorldObject*>(another);
        logic((self, target));
    };
}

class ScriptCollision {
  public:
    ScriptCollision (const VirtualObj& logic, WorldObject* obj) :
        logic_(logic), self_(obj) {}
    void operator () (void* data) {

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
            colobj->AddCollisionLogic(
                classname, 
                ScriptCollision(it->second, wobj));
        }
    }

    return colobj;
}

static void ApplyDescriptor(WorldObject* wobj, const VirtualObj& descriptor) {
    CollisionObject *collision = NULL, *visibility = NULL;

    if(descriptor["on_die_callbacks"]) {
        VirtualObj::List callbacks = descriptor["on_die_callbacks"].value<VirtualObj::List>();
        for (auto callback_function : callbacks) {
          wobj->AddDeathEvent([callback_function](WorldObject* obj) -> void {
              VirtualObj arg = VirtualObj(callback_function.wrapper());
              arg.set_value<WorldObject*>(obj);
              callback_function(VirtualObj::List(1, arg));
          });
        }
    }

    if(descriptor["collision"])
        collision = create_collision(wobj, descriptor["collision"]);

    if(descriptor["visibility"])
        visibility = create_collision(wobj, descriptor["visibility"]);

    if(collision || visibility)
        wobj->AddComponent(new component::Shape(collision, visibility));
}


WorldObject* Script(const std::string& script_name, const ugdk::script::VirtualObj& params) {
    VirtualObj script_generator = SCRIPT_MANAGER()->LoadModule("properties." + script_name);
    if(!script_generator) {
        fprintf(stderr, "Unable to load 'properties.%s'.\n", script_name.c_str());
        return NULL;
    }

    if(params && script_generator.wrapper() != params.wrapper()) {
        fprintf(stderr, "Received params are incompatible with script 'properties.%s'.\n", script_name.c_str());
        return NULL;
    }

    if(!script_generator["build"]) {
        fprintf(stderr, "Function 'build' not found in 'properties.%s'.\n", script_name.c_str());
        return NULL;
    }
    
    WorldObject* wobj = new WorldObject;
    wobj->set_identifier(script_name);

    VirtualObj v_wobj(script_generator["build"].wrapper());
    v_wobj.set_value<WorldObject*>(wobj);
    
    VirtualObj::List args(1, v_wobj);
    if(params)
        args.push_back(params);
    else // If we don't have params, place an empty vobj of the correct language there.
        args.emplace_back(v_wobj.wrapper());

    VirtualObj build_result = script_generator["build"](args);
    if(build_result)
        ApplyDescriptor(wobj, build_result);

    return wobj;
}


} // namespace ScriptBuilder
} // namespace builder
