#include "game/context.h"

#include <cstdio>
#include <vector>
#include <ugdk/math/vector2D.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>
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

WorldObject* BuildWorldObject (const string& scriptname) {
    return Script(vector<string>(1,scriptname));
}

CollisionObject* MakeCollisionObject (WorldObject* obj) {
    World *world = WORLD();
    if (!world) return NULL;
    CollisionManager *manager = world->collision_manager();
    if (!manager) return NULL;
    return new CollisionObject(manager, static_cast<void*>(obj));
}

void AddCollisionClass (const string& classname) {
    World *world = WORLD();
    assert(world);
    CollisionManager *manager = world->collision_manager();
    assert(manager);
    manager->Generate(classname);
}

void AddCollisionClass (const string& classname, const std::string& supername) {
    World *world = WORLD();
    assert(world);
    CollisionManager *manager = world->collision_manager();
    assert(manager);
    manager->Generate(classname, supername);
}

void ActivateRoom(const std::string& roomname) {
    World *world = WORLD();
    assert(world);
    world->ActivateRoom(roomname);
}

void DeactivateRoom(const std::string& roomname) {
    World *world = WORLD();
    assert(world);
    world->DeactivateRoom(roomname);
}

} // namespace context
