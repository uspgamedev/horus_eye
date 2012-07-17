#include <functional>
#include <ugdk/graphic/node.h>
#include <ugdk/script/scriptmanager.h>
#include <ugdk/script/virtualobj.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/genericcollisionlogic.h>

#include "scriptbuilder.h"

#include "game/components/graphic.h"
#include "game/components/logic.h"
#include "game/components/damageable.h"
#include "game/sprites/worldobject.h"
#include "game/scenes/world.h"

namespace builder {
namespace ScriptBuilder {
    
using ugdk::script::VirtualObj;
using sprite::WorldObject;
using std::tr1::bind;
using pyramidworks::collision::CollisionObject;
using pyramidworks::collision::CollisionLogic;
using namespace std::tr1::placeholders;

// TODO: variable damage
static void DealDamage(void* data) {
    static_cast<WorldObject*>(data)->damageable()->TakeDamage(100);
}

static CollisionLogic* create_cologic_deal_damage(WorldObject* wobj, VirtualObj argument) {
    return new pyramidworks::collision::GenericCollisionLogic(DealDamage);
}

typedef CollisionLogic* (*CollisionLogicGenerator)(WorldObject*, VirtualObj);
std::map<std::string, CollisionLogicGenerator> CollisionLogicGeneratorsPopulator() {
    std::map<std::string, CollisionLogicGenerator> result;
    result["deal_damage"] = create_cologic_deal_damage;
    // TODO: MOAR!
    return result;
}
static std::map<std::string, CollisionLogicGenerator> collision_logic_generators = CollisionLogicGeneratorsPopulator();

static void create_collision(WorldObject* wobj, VirtualObj coldata) {
    if(!coldata["class"] || !coldata["shape"]) return;

    CollisionObject* colobj = new CollisionObject(WORLD()->collision_manager(), wobj);
    wobj->set_collision_object(colobj);

    colobj->InitializeCollisionClass(coldata["class"].value<std::string>());
    colobj->set_shape(coldata["shape"].value<pyramidworks::geometry::GeometricShape*>(true));

    if(coldata["known_collision"]) {
        VirtualObj::Vector known_collisions = coldata["known_collision"].value<VirtualObj::Vector>();
        for(VirtualObj::Vector::iterator it = known_collisions.begin(); it != known_collisions.end(); ++it) {
            VirtualObj::Vector logicvect = it->value<VirtualObj::Vector>();
            if(logicvect.size() < 3) continue;
            std::string classname = logicvect[0].value<std::string>();
            std::string logicname = logicvect[1].value<std::string>();
            VirtualObj argument = logicvect[2];
            if(collision_logic_generators.find(logicname) == collision_logic_generators.end()) {
                // TODO: print warning
                // Unknown logic.
                continue;
            }
            CollisionLogic* logic = collision_logic_generators[logicname](wobj, argument);
            colobj->AddCollisionLogic(classname, logic);
        }
    }
}

static void create_drawable(WorldObject* wobj, VirtualObj data) {
    wobj->graphic()->node()->set_drawable(data.value<ugdk::graphic::Drawable*>(true));
}
static void create_timedlife(WorldObject* wobj, VirtualObj data) {
    wobj->set_timed_life(data.value<double>());
}

static void On_die_callback(WorldObject* wobj, VirtualObj vobj) {
    // TODO: send the wobj as a VirtualObj to the vobj.
    vobj();
}
static void create_die_callback(WorldObject* wobj, VirtualObj data) {
    wobj->set_die_callback(bind(On_die_callback, _1, data));
}

#define NUM_FIELDS 4
typedef void (*ScriptWobj)(WorldObject*, VirtualObj);
struct {
    std::string name;
    ScriptWobj func;
} valid_names[NUM_FIELDS] = {
    {        "drawable", create_drawable     },
    {      "timed_life", create_timedlife    },
    { "on_die_callback", create_die_callback },
    {       "collision", create_collision    }
};


/** arguments[0] is the script name. */
WorldObject* Script(const std::vector<std::string>& arguments) {
    VirtualObj script_generator = SCRIPT_MANAGER()->LoadModule(arguments[0]);
    if(!script_generator) {
        fprintf(stderr, "Unable to load script '%s'\n", arguments[0].c_str());
        return NULL;
    }
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
