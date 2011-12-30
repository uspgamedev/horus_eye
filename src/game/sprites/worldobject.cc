#include <ugdk/graphic/light.h>
#include <pyramidworks/geometry/circle.h>

#include "worldobject.h"

#include "game/scenes/world.h"
#include "game/utils/tile.h"
#include "game/utils/constants.h"

namespace sprite {

using namespace ugdk;
using namespace scene;
using namespace utils;

WorldObject::WorldObject()
    : collision_object_(NULL),
      status_(STATUS_ACTIVE),
      node_(new Node),
      identifier_("Generic World Object"),
      light_radius_(0.0f) {
}

WorldObject::~WorldObject() {
    if(collision_object_ != NULL)
        delete collision_object_;
    delete node_;
}

void WorldObject::StartToDie() {
    status_ = STATUS_DYING;
    if(collision_object_ != NULL)
        collision_object_->StopColliding();
}

void WorldObject::Update(float dt) {
    node_->set_zindex(World::FromWorldLinearCoordinates(world_position()).y); // Seta zindex
}

void WorldObject::set_light_radius(float radius) {
    light_radius_ = radius;
	if(light_radius_ > Constants::LIGHT_RADIUS_THRESHOLD) {
        if(node_->light() == NULL) node_->set_light(new Light);
		Vector2D dimension = World::ConvertLightRadius(light_radius_);
		node_->light()->set_dimension(dimension);

	} else {
		if(node_->light()) {
			delete node_->light();
			node_->set_light(NULL);
		}
	}
}

void WorldObject::set_world_position(const ugdk::Vector2D& pos) {
   world_position_ = pos;
   Vector2D position = World::FromWorldCoordinates(world_position_);
   node_->modifier()->set_offset(position);
}

void WorldObject::set_shape(pyramidworks::geometry::GeometricShape* shape) {
    collision_object_->set_shape(shape);
    shape->set_position(&world_position_);
}

}  // namespace sprite
