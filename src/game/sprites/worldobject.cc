#include "worldobject.h"

#include <cassert>
#include <ugdk/graphic/node.h>
#include <ugdk/time/timeaccumulator.h>
#include <pyramidworks/collision/collisionobject.h>

#include "game/constants.h"
#include "game/components/base.h"
#include "game/scenes/world.h"
#include "game/map/tile.h"
#include "game/map/room.h"
#include "game/components/graphic.h"

#include "game/components/damageable.h"
#include "game/components/controller.h"
#include "game/components/caster.h"
#include "game/components/shape.h"


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

WorldObject::WorldObject(double duration)
    :   identifier_("Generic World Object"),
        current_room_(NULL),
        timed_life_(NULL),
        dead_(false) {
    if(duration > 0.0)
        this->set_timed_life(duration);
}

WorldObject::~WorldObject() {
    delete timed_life_;
    for(ComponentsByOrder::const_iterator it = components_order_.begin(); it != components_order_.end(); ++it)
        delete it->component;
}

void WorldObject::Die() {
    if(graphic()) graphic()->set_visible(false);
    dead_ = true;
    if(!tag_.empty() && current_room_) current_room_->RemoveTag(tag_);
    to_be_removed_ = true;
    if(on_die_callback_) on_die_callback_(this);
}

void WorldObject::StartToDie() {
    dead_ = true;
    if(shape()) shape()->Deactivate();
    if(on_start_to_die_callback_) on_start_to_die_callback_(this);
    if(!HasComponent("animation")) Die();
}

void WorldObject::Update(double dt) {
    if(timed_life_ && timed_life_->Expired())
        StartToDie();

    for(ComponentsByOrder::const_iterator it = components_order_.begin(); it != components_order_.end(); ++it)
        it->component->Update(dt);
}

void WorldObject::set_world_position(const ugdk::Vector2D& pos) {
   world_position_ = pos;
   if(shape()) shape()->ChangePosition(pos);
   if(graphic()) graphic()->SetPosition(world_position_);
}

void WorldObject::set_timed_life(double duration) {
    #define SECONDS_TO_MILISECONDS(sec) (int)((sec) * 1000)
    delete timed_life_;
    timed_life_ = (new ugdk::time::TimeAccumulator(SECONDS_TO_MILISECONDS(duration)));
}

void WorldObject::OnRoomAdd(map::Room* room) {
    if(shape()) shape()->Activate();
    current_room_ = room;
    if(on_room_add_callback_)
        on_room_add_callback_(this, room);
}

void WorldObject::AddComponent(component::Base* component, const std::string& name, int order) {
    assert(component != NULL);
    assert(components_.find(name) == components_.end());
    assert(std::find(components_order_.begin(), components_order_.end(), component) == components_order_.end());

    OrderedComponent newcomp(component, order);
    ComponentsByOrder::iterator it;
    for(it = components_order_.begin(); it != components_order_.end() && it->order <= newcomp.order; ++it) continue;
    components_[name] = components_order_.insert(it, newcomp);
}

void WorldObject::RemoveComponent(const std::string& name) {
    ComponentsByName::const_iterator it = components_.find(name);
    if(it == components_.end()) return;
    components_order_.erase(it->second);
    components_.erase(it);
}

component::Damageable* WorldObject::damageable() { return component<component::Damageable>(); }
component::BaseGraphic* WorldObject::graphic() { return component<component::BaseGraphic>(); }
component::Controller* WorldObject::controller() { return component<component::Controller>(); }
component::Caster* WorldObject::caster() { return component<component::Caster>(); }
component::Shape* WorldObject::shape() { return component<component::Shape>(); }

}  // namespace sprite
