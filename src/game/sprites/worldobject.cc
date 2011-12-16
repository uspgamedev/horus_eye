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
      identifier_("Generic World Object"),
      light_radius_(0.0f) {
}

WorldObject::~WorldObject() {
    if(collision_object_ != NULL) 
        delete collision_object_;
}

void WorldObject::StartToDie() {
    status_ = STATUS_DYING;
    if(collision_object_ != NULL)
        collision_object_->StopColliding();
}

void WorldObject::Update(float dt) {
    Sprite::Update(dt);
    set_zindex(World::FromWorldLinearCoordinates(world_position()).y); // Seta zindex
}

void WorldObject::set_light_radius(float radius) {
    light_radius_ = radius;
	if(light_radius_ > Constants::LIGHT_RADIUS_THRESHOLD) {
		if(!light_) {
			light_ = new Light;
		}
		Vector2D dimension = World::ConvertLightRadius(light_radius_);
		light_->set_dimension(dimension);

	} else {
		if(light_) {
			delete light_;
			light_ = NULL;
		}
	}
}

void WorldObject::set_world_position(const ugdk::Vector2D& pos) {
   world_position_ = pos;
   set_position(World::FromWorldCoordinates(world_position_));
}


void WorldObject::Render() {
    if(false) { // TODO: fixme
        World *world = WORLD();
        GameMap& map = world->level_matrix();

        TilePos pos = Tile::ToTilePos(world_position());
        pos.i = map.size() - pos.i - 1;
        Tile*   obj_tile = Tile::GetFromMapPosition(world->level_matrix(), pos);
        if (obj_tile && obj_tile->visible()) {

        }
    }
    Sprite::Render();

}

void WorldObject::set_shape(pyramidworks::geometry::GeometricShape* shape) {
    collision_object_->set_shape(shape);
    shape->set_position(&world_position_);
}

}  // namespace sprite
