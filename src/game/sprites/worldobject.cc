#include "worldobject.h"

#include <cassert>
#include <ugdk/graphic/light.h>
#include <ugdk/graphic/node.h>
#include <ugdk/time/timeaccumulator.h>
#include <pyramidworks/collision/collisionobject.h>

#include "game/components/base.h"
#include "game/scenes/world.h"
#include "game/map/tile.h"
#include "game/map/room.h"
#include "game/utils/constants.h"
#include "game/sprites/condition.h"
#include "game/components/graphic.h"
#include "game/components/logic.h"

#define LIGHT_COEFFICIENT 0.75

namespace sprite {

using namespace ugdk;
using namespace scene;
using namespace utils;
using std::string;
using std::list;

WorldObject::WorldObject(double duration)
    :   identifier_("Generic World Object"),
        collision_object_(NULL),
        timed_life_(NULL),
        status_(STATUS_ACTIVE),
        current_room_(NULL),
        light_radius_(0.0),
        layer_(scene::FOREGROUND_LAYER),
        sight_count_(0) {
    if(duration > 0.0)
        this->set_timed_life(duration);
}

WorldObject::~WorldObject() {
    if(collision_object_ != NULL)
        delete collision_object_;
    if(timed_life_) delete timed_life_;

    for(ComponentsByOrder::const_iterator it = components_order_.begin(); it != components_order_.end(); ++it)
        delete *it;
}

void WorldObject::Die() {
    node()->modifier()->set_visible(false);
    status_ = STATUS_DEAD;
    if(!tag_.empty() && current_room_) current_room_->RemoveTag(tag_);
    to_be_removed_ = true;
    if(on_die_callback_) on_die_callback_(this);
}

void WorldObject::StartToDie() {
    status_ = STATUS_DYING;
    if(collision_object_) collision_object_->StopColliding();
    if(on_start_to_die_callback_) on_start_to_die_callback_(this);
}

void WorldObject::Update(double dt) {
    if(timed_life_ && timed_life_->Expired())
        StartToDie();

    UpdateCondition(dt);
    for(ComponentsByOrder::const_iterator it = components_order_.begin(); it != components_order_.end(); ++it)
        (*it)->Update(dt);
}

void WorldObject::set_world_position(const ugdk::Vector2D& pos) {
   world_position_ = pos;
   if(collision_object_) collision_object_->MoveTo(pos);

   Vector2D position = World::FromWorldCoordinates(world_position_);
   graphic()->node()->modifier()->set_offset(position);
}

void WorldObject::set_light_radius(double radius) {
    light_radius_ = radius;
    
    if(light_radius_ > Constants::LIGHT_RADIUS_THRESHOLD) {
        if(graphic()->node()->light() == NULL) graphic()->node()->set_light(new ugdk::graphic::Light);
        Vector2D dimension = World::ConvertLightRadius(light_radius_);
        graphic()->node()->light()->set_dimension(dimension * LIGHT_COEFFICIENT);

    } else {
        if(graphic()->node()->light()) {
            delete graphic()->node()->light();
            graphic()->node()->set_light(NULL);
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
    if(collision_object() != NULL)
        collision_object()->StartColliding();
    current_room_ = room;
    if(logic())
        logic()->OnRoomAdd(room);
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

void WorldObject::AddComponent(component::Base* component) {
    assert(component != NULL);
    assert(components_.find(component->name()) == components_.end());
    components_[component->name()] = component;
    components_order_.push_back(component);
}

void WorldObject::RemoveComponent(component::Base* component) {
    if(!component) return;
    ComponentsByName::const_iterator it = components_.find(component->name());
    if(it == components_.end()) return;
    components_.erase(it);
    components_order_.remove(component);
}

void WorldObject::RemoveComponent(const std::string& name) {
    ComponentsByName::const_iterator it = components_.find(name);
    if(it == components_.end()) return;
    components_order_.remove(it->second);
    components_.erase(it);
}

}
// OLD
#include "game/components/damageable.h"
#include "game/components/graphic.h"
#include "game/components/controller.h"
#include "game/components/animation.h"
#include "game/components/caster.h"

namespace sprite {

component::Logic* WorldObject::logic() { return component<component::Logic>("logic"); }
component::Damageable* WorldObject::damageable() { return component<component::Damageable>("damageable"); }
component::Graphic* WorldObject::graphic() { return component<component::Graphic>("graphic"); }
component::Controller* WorldObject::controller() { return component<component::Controller>("controller"); }
component::Animation* WorldObject::animation() { return component<component::Animation>("animation"); }
component::Caster* WorldObject::caster() { return component<component::Caster>("caster"); }

}  // namespace sprite
