
#include <cstdio>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>
#include "game/context.h"
#include "game/scenes/world.h"

namespace context {

using std::string;
using pyramidworks::collision::CollisionObject;
using pyramidworks::collision::CollisionManager;
using sprite::WorldObject;
using scene::World;

WorldObject* WorldObjectByTag (const std::string& tag) {
    return WORLD()->WorldObjectByTag(tag);
}

void CreateTag (WorldObject* obj, const std::string& tag) {
    World *world = WORLD();
    if (world)
        world->CreateTag(obj, tag);
    else
        fprintf(stderr, "Attempt to create tag in NULL world.\n");
}

CollisionObject* MakeCollisionObject (WorldObject* obj) {
    World *world = WORLD();
    if (!world) return NULL;
    CollisionManager *manager = world->collision_manager();
    if (!manager) return NULL;
    return new CollisionObject(manager, static_cast<void*>(obj));
}

} // namespace context
