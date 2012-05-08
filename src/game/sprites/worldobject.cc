#include <ugdk/graphic/light.h>
#include <ugdk/graphic/node.h>
#include <ugdk/time/timeaccumulator.h>
#include <pyramidworks/collision/collisionobject.h>

#include "worldobject.h"

#include "game/components/logic.h"
#include "game/components/damageable.h"
#include "game/components/graphic.h"
#include "game/components/controller.h"
#include "game/scenes/world.h"
#include "game/utils/tile.h"
#include "game/utils/constants.h"

#define LIGHT_COEFFICIENT 0.75

namespace sprite {

using namespace ugdk;
using namespace scene;
using namespace utils;

WorldObject::WorldObject(double duration)
    :   identifier_("Generic World Object"),
        collision_object_(NULL),
        timed_life_(NULL),
        on_death_start_callback_(NULL),
        status_(STATUS_ACTIVE),
        light_radius_(0.0),
        logic_(NULL),
        damageable_(NULL), 
        graphic_(NULL),
        controller_(NULL) {
            if(duration > 0.0) 
                this->set_timed_life(duration);
            graphic_ = new component::Graphic(this);
}

WorldObject::~WorldObject() {
    if(collision_object_ != NULL)
        delete collision_object_;
    if(timed_life_) delete timed_life_;
    if(on_death_start_callback_) delete on_death_start_callback_;
    if(logic_) delete logic_;
    if(damageable_) delete damageable_;
    if(graphic_) delete graphic_;
    if(controller_) delete controller_;
}

void WorldObject::StartToDie() {
    status_ = STATUS_DYING;
    if(collision_object_ != NULL)
        collision_object_->StopColliding();
    if(on_death_start_callback_)
        on_death_start_callback_->Callback();
}

void WorldObject::Update(double dt) {
    if(timed_life_ && timed_life_->Expired())
        StartToDie();

    if(controller_) controller_->Update(dt);
    if(damageable_) damageable_->Update(dt);
    if(logic_) logic_->Update(dt);
    if(graphic_) graphic_->Update(dt);

    if(status_ == STATUS_DYING) 
        Dying(dt);
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
    scene->content_node()->AddChild(node());
    if(collision_object() != NULL)
        collision_object()->StartColliding();
}

}  // namespace sprite
