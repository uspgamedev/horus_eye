#include "game/core/coordinates.h"

#include <ugdk/graphic/node.h>
#include "game/scenes/world.h"

namespace core {

using ugdk::math::Vector2D;

static Vector2D tile_size(106, 54);

Vector2D FromScreenLinearCoordinates(const Vector2D& screen_coords) {
    static Vector2D tx = Vector2D( 1.0 / tile_size.x, -1.0 / tile_size.x);
    static Vector2D ty = Vector2D(-1.0 / tile_size.y, -1.0 / tile_size.y);
    return (tx * screen_coords.x) + (ty * screen_coords.y);
}

Vector2D FromWorldLinearCoordinates(const Vector2D& world_coords) {
    static Vector2D tx = Vector2D( tile_size.x / 2.0, -tile_size.y / 2.0);
    static Vector2D ty = Vector2D(-tile_size.x / 2.0, -tile_size.y / 2.0);
    return (tx * world_coords.x) + (ty * world_coords.y);
}

Vector2D FromWorldCoordinates(const Vector2D& world_coords) {
    return FromWorldLinearCoordinates(world_coords);
}

Vector2D FromScreenCoordinates(scene::World* world, const Vector2D& screen_coords) {
    const ugdk::graphic::Geometry& camera = world->camera();
    return FromScreenLinearCoordinates((screen_coords - camera.offset()) / camera.CalculateScale().x);
}
    
} // namespace core
