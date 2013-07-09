
#ifndef HORUSEYE_CONTEXT_H_
#define HORUSEYE_CONTEXT_H_

#include <string>
#include <vector>

#include <ugdk/math.h>
#include <pyramidworks/collision.h>
#include <pyramidworks/geometry.h>

#include "game/map.h"
#include "game/sprites/worldobject.h"

namespace context {
    
sprite::WorldObject* WorldObjectByTag (const std::string& tag);

pyramidworks::collision::CollisionObject* MakeCollisionObject (sprite::WorldObject* obj);

void AddCollisionClass (const std::string& classname);

void AddCollisionClass (const std::string& classname, const std::string& supername);

void ActivateRoom(const std::string& roomname);

void DeactivateRoom(const std::string& roomname);

void AddDamageableComponent(const std::string& tag, double life);
void AddDamageableComponent(const map::Room*, const std::string& tag, double life);

/// Appends to the objects_colliding list all WorldObjects from the given collision class (classname) that are colliding with
/// the given GeometricShape.
void GetCollidingObjects(const std::string& classname, const pyramidworks::geometry::GeometricShape& shape, 
						 const ugdk::math::Vector2D& pos, std::vector<sprite::WorldObject*> &objects_colliding);

void GetCollidingVisibilityObjects(const std::string& classname, const pyramidworks::geometry::GeometricShape& shape, 
								   const ugdk::math::Vector2D& pos, std::vector<sprite::WorldObject*> &objects_colliding);

sprite::WorldObject* hero();

} // namespace context


#endif /* HORUSEYE_CONTEXT_H_ */
