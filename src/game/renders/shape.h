#ifndef HORUSEYE_RENDERS_SHAPE_H_
#define HORUSEYE_RENDERS_SHAPE_H_

#include <pyramidworks/geometry.h>
#include <pyramidworks/collision.h>
#include <ugdk/graphic.h>
#include <ugdk/math.h>

namespace renders {

void DrawShape(const pyramidworks::geometry::GeometricShape*, const ugdk::math::Vector2D& position,
               const ugdk::graphic::Geometry& geometry, const ugdk::graphic::VisualEffect& effect);

void DrawCollisionObject(const pyramidworks::collision::CollisionObject*, const ugdk::graphic::Geometry& geometry, const ugdk::graphic::VisualEffect& effect);

} // namespace renders

#endif