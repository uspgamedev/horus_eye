#include "worldobject.h"

#include <cassert>
#include <ugdk/graphic/light.h>
#include <ugdk/graphic/node.h>
#include <ugdk/time/timeaccumulator.h>
#include <pyramidworks/collision/collisionobject.h>

#include "game/constants.h"
#include "game/components/base.h"
#include "game/scenes/world.h"
#include "game/map/tile.h"
#include "game/map/room.h"
#include "game/sprites/condition.h"
#include "game/components/graphic.h"

#include "game/components/damageable.h"
#include "game/components/controller.h"
#include "game/components/caster.h"

#define LIGHT_COEFFICIENT 0.75

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
        collision_object_(NULL),
        visibility_object_(NULL),
        timed_life_(NULL),
        status_(STATUS_ACTIVE),
        current_room_(NULL),
        light_radius_(0.0),
        layer_(scene::FOREGROUND_LAYER),
        sight_count_(0) {
    if(duration > 0.0)
        this->set_timed_life(duration);

    AddComponent(new component::Graphic(this));
}

WorldObject::~WorldObject() {
    delete collision_object_;
    delete visibility_object_;
    delete timed_life_;
    for(ComponentsByOrder::const_iterator it = components_order_.begin(); it != components_order_.end(); ++it)
        delete it->component;
}

void WorldObject::Die() {
    node()->modifier()->set_visible(false);
    status_ = STATUS_DEAD;
    if(!tag_.empty() && current_room_) current_room_->RemoveTag(tag_);
    to_be_removed_ = true;
    if(on_die_callback_) on_die_callback_(this);
    if(visibility_object_) visibility_object_->StopColliding();
}

void WorldObject::StartToDie() {
    status_ = STATUS_DYING;
    if(collision_object_) collision_object_->StopColliding();
    if(on_start_to_die_callback_) on_start_to_die_callback_(this);
    if(!HasComponent("animation")) Die();
}

void WorldObject::Update(double dt) {
    if(timed_life_ && timed_life_->Expired())
        StartToDie();

    UpdateCondition(dt);
    for(ComponentsByOrder::const_iterator it = components_order_.begin(); it != components_order_.end(); ++it)
        it->component->Update(dt);
}

void WorldObject::set_world_position(const ugdk::Vector2D& pos) {
   world_position_ = pos;
   if(collision_object_) collision_object_->MoveTo(pos);
   if(visibility_object_) visibility_object_->MoveTo(pos);

   Vector2D position = World::FromWorldCoordinates(world_position_);
   graphic()->node()->modifier()->set_offset(position);
   graphic()->node()->set_zindex(position.y);
}

void WorldObject::set_light_radius(double radius) {
    light_radius_ = radius;
    ugdk::graphic::Node* node = graphic()->node();
    
    if(light_radius_ > constants::GetDouble("LIGHT_RADIUS_THRESHOLD")) {
        if(node->light() == NULL) 
            node->set_light(new ugdk::graphic::Light);

        Vector2D dimension = World::ConvertLightRadius(light_radius_);
        node->light()->set_dimension(dimension * LIGHT_COEFFICIENT);

    } else {
        if(node->light()) {
            delete node->light();
            node->set_light(NULL);
        }
    }
}

void WorldObject::set_shape(pyramidworks::geometry::GeometricShape* shape) {
    collision_object_->set_shape(shape);
}

ugdk::graphic::Node* WorldObject::node() { 
    return graphic()->node();
}

const ugdk::graphic::Node* WorldObject::node() const {
    return component<component::Graphic>("graphic")->node();
}

void WorldObject::set_timed_life(ugdk::time::TimeAccumulator* timer) {
    if(timed_life_) delete timed_life_;
    timed_life_ = timer;
}

void WorldObject::set_timed_life(double duration) {
    #define SECONDS_TO_MILISECONDS(sec) (int)((sec) * 1000)
    set_timed_life(new ugdk::time::TimeAccumulator(SECONDS_TO_MILISECONDS(duration)));
}

void WorldObject::OnRoomAdd(map::Room* room) {
    if(collision_object())
        collision_object()->StartColliding();
    if(visibility_object_)
        visibility_object_->StartColliding();
    current_room_ = room;
    if(on_room_add_callback_)
        on_room_add_callback_(this, room);
}

static bool deletecondition(Condition *condition) {
    bool is_finished = (condition->phase() == Condition::PHASE_FINISHED);
    if (is_finished) delete condition;
    return is_finished;
}

bool WorldObject::AddCondition(Condition* new_condition) {
    conditions_.push_front(new_condition);
    new_condition->StartCondition(this);
    return true;
}

void WorldObject::UpdateCondition(double dt) {
     std::list<Condition*>::iterator i;
     for (i = conditions_.begin(); i != conditions_.end(); ++i)
         (*i)->Update(dt);
     conditions_.remove_if(deletecondition);
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
component::Graphic* WorldObject::graphic() { return component<component::Graphic>(); }
component::Controller* WorldObject::controller() { return component<component::Controller>(); }
component::Caster* WorldObject::caster() { return component<component::Caster>(); }

}  // namespace sprite
