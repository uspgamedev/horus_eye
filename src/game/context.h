
#ifndef HORUSEYE_CONTEXT_H_
#define HORUSEYE_CONTEXT_H_

#include <string>
#include <vector>

#include <ugdk/math.h>
#include <pyramidworks/geometry.h>

#include "game/sprites/worldobject.h"
#include "game/map.h"
#include "game/ai.h"

namespace context {
    
sprite::WObjWeakPtr WorldObjectByTag (const std::string& tag);

void AddDamageableComponent(const std::string& tag, double life);
void AddDamageableComponent(const map::Room*, const std::string& tag, double life);
void AddAIComponent(sprite::WorldObject* wobj, ai::AI* the_ai);
void EnableDeathAnimation(sprite::WorldObject* wobj);

/// Appends to the objects_colliding list all WorldObjects from the given collision class (classname) that are colliding with
/// the given GeometricShape.
void GetCollidingObjects(const std::string& classname,
                         const pyramidworks::geometry::GeometricShape& shape, 
						 const ugdk::math::Vector2D& pos,
                         std::vector<sprite::WorldObject*> &objects_colliding);

void GetCollidingVisibilityObjects(const std::string& classname,
                                   const pyramidworks::geometry::GeometricShape& shape, 
								   const ugdk::math::Vector2D& pos,
                                   std::vector<sprite::WorldObject*> &objects_colliding);

sprite::WObjWeakPtr hero();

} // namespace context


#endif /* HORUSEYE_CONTEXT_H_ */
