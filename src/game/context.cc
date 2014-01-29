#include "game/context.h"

#include <cstdio>
#include <vector>
#include <ugdk/math/vector2D.h>
#include <pyramidworks/collision/collisionobject.h>
#include <pyramidworks/collision/collisionclass.h>
#include <pyramidworks/collision/collisionmanager.h>
#include "game/scenes/world.h"
#include "game/builders/scriptbuilder.h"
#include "game/builders/aibuilder.h"
#include "game/sprites/objecthandle.h"
#include "game/map/room.h"
#include "game/components/damageable.h"
#include "game/components/animation.h"
#include "game/ai/ai.h"

namespace context {

using std::string;
using std::vector;
using ugdk::math::Vector2D;
using pyramidworks::collision::CollisionObject;
using pyramidworks::collision::CollisionManager;
using pyramidworks::collision::CollisionObjectList;
using pyramidworks::collision::CollisionClass;
using pyramidworks::geometry::GeometricShape;
using sprite::WorldObject;
using scene::World;
using builder::ScriptBuilder::Script;
using component::Animation;

sprite::WObjWeakPtr WorldObjectByTag (const std::string& tag) {
    World *world = WORLD();
    assert(world);
    size_t slashpos = tag.find_first_of('/');
    if(slashpos == std::string::npos) {
        fprintf(stderr, "Tag '%s' is not of the format 'X/Y'\n", tag.c_str());
        return sprite::WObjWeakPtr();
    }
    std::string room_name = tag.substr(0, slashpos),
        obj_tag = tag.substr(slashpos);

    const map::Room* room = world->GetRoom(room_name);
    if(room)
        return room->WorldObjectByTag(obj_tag);
    else
        return sprite::WObjWeakPtr();
}

static void _internal_AddDamageableComponent(const sprite::WObjPtr& obj, double life) {
    component::Damageable* damageable = new component::Damageable(obj.get());
    damageable->life() = resource::Energy(life);
    obj->AddComponent(damageable);
}

void AddDamageableComponent(const std::string& tag, double life) {
    sprite::WObjPtr obj = WorldObjectByTag(tag).lock();
    if(!obj) {
        fprintf(stderr, "No object with tag '%s' found.\n", tag.c_str());
        return;
    }
    _internal_AddDamageableComponent(obj, life);
}

void AddDamageableComponent(const map::Room* room, const std::string& tag, double life) {
    sprite::WObjPtr obj = room->WorldObjectByTag(tag).lock();
    if(!obj) {
        fprintf(stderr, "No object with tag '%s' in room '%s' found.\n", tag.c_str(), room->name().c_str());
        return;
    }
    _internal_AddDamageableComponent(obj, life);
}

void AddAIComponent(WorldObject* wobj, ai::AI* the_ai) {
    wobj->AddComponent(the_ai);
}

void EnableDeathAnimation(const sprite::ObjectHandle& handle) {
    if (!handle.attached()) return;

    handle->set_start_to_die_callback([](WorldObject* wobj) -> void {
        wobj->component<Animation>()->ChangeAnimation(utils::DEATH);
    });
    handle->component<Animation>()->AddCallback(utils::DEATH, std::mem_fn(&WorldObject::Remove));
}

static void findCollisions(CollisionClass &colclass, const GeometricShape& shape, const Vector2D& pos, vector<WorldObject*>& objects_colliding) {
    CollisionObjectList result;
    colclass.FindCollidingObjects(pos, shape, result);
    for(const CollisionObject * obj : result)
        if(WorldObject* wobj = dynamic_cast<WorldObject*>(obj->owner()))
            objects_colliding.push_back(wobj);
}

void GetCollidingObjects(const string& classname, const GeometricShape& shape, const Vector2D& pos, vector<WorldObject*> &objects_colliding) {
    World *world = WORLD();
    if (!world) return;
    CollisionClass &colclass = world->collision_manager()->Find(classname);
    findCollisions(colclass, shape, pos, objects_colliding);
}

void GetCollidingVisibilityObjects(const string& classname, const GeometricShape& shape, const Vector2D& pos, vector<WorldObject*>& objects_colliding) {
    World *world = WORLD();
    if (!world) return;
    CollisionClass &colclass = world->visibility_manager()->Find(classname);
    findCollisions(colclass, shape, pos, objects_colliding);
}

sprite::WObjWeakPtr hero() {
    World *world = WORLD();
    if (!world) return sprite::WObjWeakPtr();
    return world->hero();
}

} // namespace context
