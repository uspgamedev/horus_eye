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

bool WorldObject::OrderedComponent::operator == (const component::Base* base) const {
    return component == base;
}

WorldObject::OrderedComponent::OrderedComponent(component::Base* base, int _order) : component(base), order(_order) {}

WObjPtr WorldObject::Create() {
    return WObjPtr(new WorldObject);
}

WorldObject::WorldObject()
    : identifier_("Generic World Object")
    , current_room_(nullptr)
    , dead_(false)
    , to_be_removed_(false)
{}

WorldObject::~WorldObject() {
    for(ComponentsByOrder::const_iterator it = components_order_.begin(); it != components_order_.end(); ++it)
        delete it->component;
}

void WorldObject::Remove() {
    if(auto g = graphic())
        g->set_visible(false);
    dead_ = true;
    if(!tag_.empty() && current_room_) current_room_->RemoveTag(tag_);
    to_be_removed_ = true;
    for (const auto& callback : on_die_callbacks_)
        callback(this);
}

void WorldObject::Die() {
    dead_ = true;
    if(auto b = body())
        b->Deactivate();
    if(on_start_to_die_callback_)
        on_start_to_die_callback_(this);
    if(!HasComponent("animation"))
        Remove();
}

void WorldObject::Update(double dt) {
    for(const auto& it : components_order_)
        it.component->Update(dt);
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

void WorldObject::AddComponent(component::Base* component, const std::string& name, int order) {
    assert(component != nullptr);
    assert(components_.find(name) == components_.end());
    assert(std::find(components_order_.begin(), components_order_.end(), component) == components_order_.end());

    OrderedComponent newcomp(component, order);
    ComponentsByOrder::iterator it;
    for(it = components_order_.begin(); it != components_order_.end() && it->order <= newcomp.order; ++it) continue;
    components_[name] = components_order_.insert(it, newcomp);
    component->OnAdd(this);
}

void WorldObject::RemoveComponent(const std::string& name) {
    ComponentsByName::const_iterator it = components_.find(name);
    if(it == components_.end()) return;
    components_order_.erase(it->second);
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
