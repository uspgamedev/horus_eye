#ifndef HORUSEYE_RENDERS_SHAPE_H_
#define HORUSEYE_RENDERS_SHAPE_H_

#include <pyramidworks/geometry.h>
#include <pyramidworks/collision.h>
#include <ugdk/graphic.h>
#include <ugdk/math.h>

namespace renders {

void DrawShape(const pyramidworks::geometry::GeometricShape*, const ugdk::math::Vector2D& position,
               ugdk::graphic::Canvas&);

void DrawCollisionObject(const pyramidworks::collision::CollisionObject*, ugdk::graphic::Canvas&);

} // namespace renders

#endif