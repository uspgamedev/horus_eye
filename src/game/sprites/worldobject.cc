#include "worldobject.h"

#include <ugdk/time/timeaccumulator.h>
#include <pyramidworks/collision/collisionobject.h>

#include "game/constants.h"
#include "game/components/base.h"
#include "game/scenes/world.h"
#include "game/map/tile.h"
#include "game/map/room.h"
#include "game/components/graphic.h"
#include "game/components/lightemitter.h"
#include "game/components/body.h"

#include "game/components/damageable.h"
#include "game/components/controller.h"
#include "game/components/caster.h"
#include "game/components/animation.h"
#include "game/components/animator.h"

#include <cassert>

namespace sprite {

using namespace ugdk;
using namespace scene;
using namespace utils;
using std::string;
using std::list;

WObjPtr WorldObject::Create() {
    return WObjPtr(new WorldObject);
}

WorldObject::WorldObject()
    : identifier_("Generic World Object")
    , current_room_(nullptr)
    , to_be_removed_(false)
{}

WorldObject::~WorldObject() {
}

void WorldObject::Remove() {
    to_be_removed_ = true;
    if (!tag_.empty() && current_room_)
        current_room_->RemoveTag(tag_);
    for (const auto& comp : components_order_)
        comp->OnObjectRemoved();
    for (const auto& callback : on_remove_callbacks_)
        callback(this);
}

void WorldObject::Update(double dt) {
    for(const auto& comp : components_order_)
        comp->Update(dt);
}

void WorldObject::set_world_position(const ugdk::math::Vector2D& pos) {
    world_position_ = pos;
    if(auto b = body()) 
        b->ChangePosition(pos);
    if(auto g = graphic()) 
        g->SetPosition(world_position_);
    if(auto l = light())
        l->SetPosition(world_position_);

    if(current_room_) {
        map::Room* new_room = current_room_->level()->FindRoomFromPoint(world_position_);
        if(new_room && new_room != current_room_)
            current_room_->level()->QueueRoomChange(shared_from_this(), new_room);
    }
}

void WorldObject::OnRoomAdd(map::Room* room) {
    current_room_ = room;
    if(on_room_add_callback_)
        on_room_add_callback_(this, room);
}

void WorldObject::AddComponent(component::Base* component) {
    assert(component != nullptr);

    std::string name = component->component_name();
    ugdk::system::AssertCondition<ugdk::system::InvalidOperation>(
        components_.find(name) == components_.end(),
        ("Object already has component with name: " + name).c_str());

    ComponentsByOrder::iterator previous, it;
    for (previous = components_order_.before_begin(), it = components_order_.begin();
         it != components_order_.end() && (*it)->order() <= component->order();
         ++previous, ++it)
        continue;
    components_[name] = components_order_.emplace_after(previous, component);
    component->OnAdd(this);
}

void WorldObject::RemoveComponent(const std::string& name) {
    ComponentsByName::const_iterator it = components_.find(name);
    if(it == components_.end()) return;

    ComponentsByOrder::iterator previous, target;
    for (previous = components_order_.before_begin(), target = components_order_.begin();
         target != components_order_.end() && target != it->second;
         ++previous, ++it)
         continue;

    components_order_.erase_after(previous);
    components_.erase(it);
}

component::Damageable* WorldObject::damageable() { return component<component::Damageable>(); }
component::Graphic* WorldObject::graphic() { return component<component::Graphic>(); }
component::Animator* WorldObject::animator() { return component<component::Animator>(); }
component::Animation* WorldObject::animation() { return component<component::Animation>(); }
component::Controller* WorldObject::controller() { return component<component::Controller>(); }
component::Caster* WorldObject::caster() { return component<component::Caster>(); }
component::Body* WorldObject::body() { return component<component::Body>(); }
component::LightEmitter* WorldObject::light() { return component<component::LightEmitter>(); }

}  // namespace sprite
