#include "worldobject.h"

#include <ugdk/graphic/light.h>
#include <ugdk/graphic/node.h>
#include <ugdk/time/timeaccumulator.h>
#include <pyramidworks/collision/collisionobject.h>

#include "game/components/logic.h"
#include "game/components/damageable.h"
#include "game/components/graphic.h"
#include "game/components/controller.h"
#include "game/components/animation.h"
#include "game/components/caster.h"
#include "game/scenes/world.h"
#include "game/utils/tile.h"
#include "game/utils/constants.h"
#include "game/sprites/condition.h"

#define LIGHT_COEFFICIENT 0.75

namespace sprite {

using namespace ugdk;
using namespace scene;
using namespace utils;

WorldObject::WorldObject(double duration)
    :   identifier_("Generic World Object"),
        collision_object_(NULL),
        timed_life_(NULL),
        status_(STATUS_ACTIVE),
        light_radius_(0.0),
        layer_(scene::FOREGROUND_LAYER),
        damageable_(NULL), 
        graphic_(NULL),
        logic_(NULL),
        controller_(NULL),
        animation_(NULL),
        caster_(NULL),
        sight_count_(0) {
    if(duration > 0.0)
        this->set_timed_life(duration);
    graphic_ = new component::Graphic(this);
}

WorldObject::~WorldObject() {
    if(collision_object_ != NULL)
        delete collision_object_;
    if(timed_life_) delete timed_life_;
    if(damageable_) delete damageable_;
    delete graphic_;
    if(logic_) delete logic_;
    if(controller_) delete controller_;
    if(animation_) delete animation_;
}

void WorldObject::Die() {
    status_ = STATUS_DEAD;
    if(!tag_.empty()) WORLD()->RemoveTag(tag_);
    to_be_removed_ = true;
    if(on_die_callback_) on_die_callback_(this);
}

void WorldObject::StartToDie() {
    status_ = STATUS_DYING;
    if(collision_object_) collision_object_->StopColliding();
    if(on_start_to_die_callback_) on_start_to_die_callback_(this);
    if(!animation_) Die();
}

void WorldObject::Update(double dt) {
    if(timed_life_ && timed_life_->Expired())
        StartToDie();

    if(controller_) controller_->Update(dt);
    if(damageable_) damageable_->Update(dt);
    if(caster_) caster_->Update(dt);
    if(logic_) logic_->Update(dt);
    UpdateCondition(dt);
    if(animation_) animation_->Update(dt);
    graphic_->Update(dt);
}

void WorldObject::set_world_position(const ugdk::Vector2D& pos) {
   world_position_ = pos;
   if(collision_object_) collision_object_->MoveTo(pos);

   Vector2D position = World::FromWorldCoordinates(world_position_);
   graphic_->node()->modifier()->set_offset(position);
   graphic_->node()->set_zindex(position.y);
}

void WorldObject::set_light_radius(double radius) {
    light_radius_ = radius;
    
    if(light_radius_ > Constants::LIGHT_RADIUS_THRESHOLD) {
        if(graphic_->node()->light() == NULL) graphic_->node()->set_light(new ugdk::graphic::Light);
        Vector2D dimension = World::ConvertLightRadius(light_radius_);
        graphic_->node()->light()->set_dimension(dimension * LIGHT_COEFFICIENT);

    } else {
        if(graphic_->node()->light()) {
            delete graphic_->node()->light();
            graphic_->node()->set_light(NULL);
        }
    }
}

void WorldObject::set_shape(pyramidworks::geometry::GeometricShape* shape) {
    collision_object_->set_shape(shape);
}

ugdk::graphic::Node* WorldObject::node() { return graphic_->node(); }
const ugdk::graphic::Node* WorldObject::node() const { return graphic_->node(); }

void WorldObject::set_timed_life(ugdk::time::TimeAccumulator* timer) {
    if(timed_life_) delete timed_life_;
    timed_life_ = timer;
}

void WorldObject::set_timed_life(double duration) {
    #define SECONDS_TO_MILISECONDS(sec) (int)((sec) * 1000)
    set_timed_life(new ugdk::time::TimeAccumulator(SECONDS_TO_MILISECONDS(duration)));
}

void WorldObject::OnSceneAdd(ugdk::action::Scene* scene) {
    World* world = static_cast<World*>(scene);
    world->layer_node(layer_)->AddChild(node());
    if(collision_object() != NULL)
        collision_object()->StartColliding();
    if(logic_)
        logic_->OnWorldAdd(world);
}

bool deletecondition(Condition *condition) {
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

}  // namespace sprite
