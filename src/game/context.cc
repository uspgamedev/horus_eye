
#include <cstdio>
#include <vector>
#include <ugdk/math/vector2D.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>
#include "game/context.h"
#include "game/scenes/world.h"
#include "game/builders/scriptbuilder.h"

namespace context {

using std::string;
using std::vector;
using ugdk::Vector2D;
using pyramidworks::collision::CollisionObject;
using pyramidworks::collision::CollisionManager;
using sprite::WorldObject;
using scene::World;
using builder::ScriptBuilder::Script;

WorldObject* WorldObjectByTag (const string& tag) {
    return WORLD()->WorldObjectByTag(tag);
}

WorldObject* BuildWorldObject (const string& scriptname) {
    return Script(vector<string>(1,scriptname));
}

void AddWorldObject (WorldObject* new_obj, const Vector2D& pos) {
    WORLD()->AddWorldObject(new_obj, pos);
}

CollisionObject* MakeCollisionObject (WorldObject* obj) {
    World *world = WORLD();
    if (!world) return NULL;
    CollisionManager *manager = world->collision_manager();
    if (!manager) return NULL;
    return new CollisionObject(manager, static_cast<void*>(obj));
}

} // namespace context
