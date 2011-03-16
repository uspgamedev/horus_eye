#include "worldobject.h"
#include "../scenes/world.h"
#include "../utils/circleobject.h"
#include "../utils/fog.h"
#include "../utils/tile.h"

namespace sprite {

using namespace framework;
using namespace scene;
using namespace utils;

WorldObject::WorldObject()
    : bound_(NULL),
      status_(STATUS_ACTIVE),
      collision_type_(NO_COLLISION),
      light_radius_(0.0f)
{}

WorldObject::~WorldObject() {
    delete bound_;
}

void WorldObject::Update(float dt) {
    Sprite::Update(dt);
    set_zindex(World::FromWorldLinearCoordinates(world_position()).y); // Seta zindex
}

void WorldObject::set_light_radius(float radius) {
    light_radius_ = radius;
    WORLD()->fog()->UpdateLightSource(this);
}

bool WorldObject::IsColliding(WorldObject* obj) const {
    return bound_->Intersects(obj->bound());
}

void WorldObject::HandleCollision(WorldObject* obj) {
//     double dispatch
//     http://en.wikipedia.org/wiki/Double_dispatch
    obj->CollidesWith(this);
}

void WorldObject::set_world_position(const framework::Vector2D& pos) {
   bound_->set_position(pos);
   set_position(World::FromWorldCoordinates(pos));
}


void WorldObject::Render(Image *back_buffer, Vector2D &offset) {
    if(false) {
        World *world = WORLD();
        GameMap& map = world->level_matrix();

        TilePos pos = Tile::ToTilePos(world_position());
        pos.i = map.size() - pos.i - 1;
        Tile*   obj_tile = Tile::GetFromMapPosition(world->level_matrix(), pos);
        if (obj_tile && obj_tile->visible()) {

        }
    }
    Sprite::Render(back_buffer, offset);

}

}  // namespace sprite
