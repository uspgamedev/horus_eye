#include "game/context.h"

#include <cstdio>
#include <vector>
#include <ugdk/math/vector2D.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionmanager.h>
#include "game/scenes/world.h"
#include "game/builders/scriptbuilder.h"
#include "game/map/room.h"
#include "game/components/damageable.h"

namespace context {

using std::string;
using std::vector;
using ugdk::Vector2D;
using pyramidworks::collision::CollisionObject;
using pyramidworks::collision::CollisionManager;
using pyramidworks::collision::CollisionInstance;
using pyramidworks::geometry::GeometricShape;
using sprite::WorldObject;
using scene::World;
using builder::ScriptBuilder::Script;


sprite::WorldObject* WorldObjectByTag (const std::string& tag) {
    World *world = WORLD();
    assert(world);
    size_t slashpos = tag.find_first_of('/');
    if(slashpos == std::string::npos) {
        fprintf(stderr, "Tag '%s' is not of the format 'X/Y'\n", tag.c_str());
        return NULL;
    }
    std::string room_name = tag.substr(0, slashpos),
        obj_tag = tag.substr(slashpos);

    const map::Room* room = world->GetRoom(room_name);
    if(room)
        return room->WorldObjectByTag(obj_tag);
    else
        return NULL;
}

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

static void _internal_AddDamageableComponent(sprite::WorldObject* obj, double life) {
    component::Damageable* damageable = new component::Damageable(obj);
    damageable->life() = resource::Energy(life);
    obj->AddComponent(damageable);
}

void AddDamageableComponent(const std::string& tag, double life) {
    sprite::WorldObject* obj = WorldObjectByTag(tag);
    if(!obj) {
        fprintf(stderr, "No object with tag '%s' found.\n", tag.c_str());
        return;
    }
    _internal_AddDamageableComponent(obj, life);
}

void AddDamageableComponent(const map::Room* room, const std::string& tag, double life) {
    sprite::WorldObject* obj = room->WorldObjectByTag(tag);
    if(!obj) {
        fprintf(stderr, "No object with tag '%s' in room '%s' found.\n", tag.c_str(), room->name().c_str());
        return;
    }
    _internal_AddDamageableComponent(obj, life);
}

static void GetCollidingObjectsRaw(CollisionManager *manager, const string& classname, GeometricShape* shape, const Vector2D& pos, vector<WorldObject*> &objects_colliding) {
    if (!manager) return;
    CollisionObject* obj = new CollisionObject(manager, NULL);
	obj->set_shape(shape);
	obj->AddCollisionLogic(classname, NULL);
    obj->MoveTo(pos);
	
	vector<CollisionInstance> col_instances;
	obj->SearchCollisions(col_instances);
	vector<CollisionInstance>::iterator it;
    for(it = col_instances.begin(); it != col_instances.end(); ++it) {
		//it->first->Handle(it->second);
		WorldObject* wobj = static_cast<WorldObject*>(it->second);
		if (wobj != NULL)	objects_colliding.push_back(wobj);
    }
}

void GetCollidingObjects(const string& classname, GeometricShape* shape, const Vector2D& pos, vector<WorldObject*> &objects_colliding) {
    World *world = WORLD();
    if (!world) return;
    CollisionManager *manager = world->collision_manager();
    GetCollidingObjectsRaw(manager, classname, shape, pos, objects_colliding);
}

void GetCollidingVisibilityObjects(const std::string& classname, pyramidworks::geometry::GeometricShape* shape, const ugdk::Vector2D& pos, std::vector<sprite::WorldObject*> &objects_colliding) {
    World *world = WORLD();
    if (!world) return;
    CollisionManager *manager = world->visibility_manager();
    GetCollidingObjectsRaw(manager, classname, shape, pos, objects_colliding);
}

sprite::WorldObject* hero() {
    World *world = WORLD();
    if (!world) return NULL;
    return world->hero();
}

} // namespace context
