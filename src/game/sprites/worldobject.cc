#include <ugdk/graphic/light.h>
#include <ugdk/graphic/node.h>
#include <ugdk/time/timeaccumulator.h>
#include <pyramidworks/collision/collisionobject.h>

#include "worldobject.h"

#include "game/components/logic.h"
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
        node_(new ugdk::graphic::Node),
        timed_life_(NULL),
        on_death_start_callback_(NULL),
        status_(STATUS_ACTIVE),
        light_radius_(0.0),
        logic_(NULL) {
            if(duration > 0.0) 
                this->set_timed_life(duration);
}

WorldObject::~WorldObject() {
    if(collision_object_ != NULL)
        delete collision_object_;
    delete node_;
    if(timed_life_) delete timed_life_;
    if(on_death_start_callback_) delete on_death_start_callback_;
    if(logic_) delete logic_;
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

    if(logic_) logic_->Update(dt);

    if(status_ == STATUS_DYING) 
        Dying(dt);
}

void WorldObject::set_world_position(const ugdk::Vector2D& pos) {
   world_position_ = pos;
   if(collision_object_) collision_object_->MoveTo(pos);

   Vector2D position = World::FromWorldCoordinates(world_position_);
   node_->modifier()->set_offset(position);
   node_->set_zindex(position.y);
}

void WorldObject::set_light_radius(double radius) {
    light_radius_ = radius;
    
	if(light_radius_ > Constants::LIGHT_RADIUS_THRESHOLD) {
        if(node_->light() == NULL) node_->set_light(new ugdk::graphic::Light);
		Vector2D dimension = World::ConvertLightRadius(light_radius_);
		node_->light()->set_dimension(dimension * LIGHT_COEFFICIENT);

	} else {
		if(node_->light()) {
			delete node_->light();
			node_->set_light(NULL);
		}
	}
}

void WorldObject::set_shape(pyramidworks::geometry::GeometricShape* shape) {
    collision_object_->set_shape(shape);
}

void WorldObject::set_timed_life(ugdk::time::TimeAccumulator* timer) {
    if(timed_life_) delete timed_life_;
    timed_life_ = timer;
}

void WorldObject::set_timed_life(double duration) {
    #define SECONDS_TO_MILISECONDS(sec) (int)((sec) * 1000)
    set_timed_life(new ugdk::time::TimeAccumulator(SECONDS_TO_MILISECONDS(duration)));
}

}  // namespace sprite
