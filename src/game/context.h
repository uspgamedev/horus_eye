#ifndef HORUSEYE_CONTEXT_H_
#define HORUSEYE_CONTEXT_H_

#include <string>
#include <vector>

#include <ugdk/action.h>
#include <ugdk/math.h>
#include <pyramidworks/geometry.h>

#include "game/sprites/worldobject.h"
#include "game/sprites/objecthandle.h"
#include "game/map.h"
#include "game/ai.h"

namespace context {
    
sprite::ObjectHandle WorldObjectByTag (const std::string& tag);

void AddDamageableComponent(const std::string& tag, double life);
void AddDamageableComponent(const map::Room*, const std::string& tag, double life);
void AddAIComponent(const sprite::ObjectHandle& wobj, ai::AI* the_ai);
void AddCollisionObjectRect(const sprite::ObjectHandle& handle, const std::string& colclass, double width,
                            double height);
void EnableDeathAnimation(const sprite::ObjectHandle& handle);

/// Appends to the objects_colliding list all WorldObjects from the given collision class (classname) that are colliding with
/// the given GeometricShape.
void GetCollidingObjects(const std::string& classname,
                         const pyramidworks::geometry::GeometricShape& shape, 
						 const ugdk::math::Vector2D& pos,
                         std::vector<sprite::WObjRawPtr> &objects_colliding);

void GetCollidingVisibilityObjects(const std::string& classname,
                                   const pyramidworks::geometry::GeometricShape& shape, 
                                   const ugdk::math::Vector2D& pos,
                                   std::vector<sprite::WObjRawPtr> &objects_colliding);

sprite::ObjectHandle hero();

void ShowScrollingText(const std::string& tag);
void ShowImageAsScene(const std::string& tag, bool is_filename = false);
void ShowTextAsScene(const std::string& message);

void ChangeConsoleLanguage(const std::string&);

void LoadTextureAtlas(const std::string& tag, const std::string& filepath);
void LoadTextureAtlas(const std::string& tag, ugdk::graphic::TextureAtlas* atlas);

sprite::WObjRawPtr ToWorldObject(pyramidworks::collision::CollisionData*);

} // namespace context


#endif /* HORUSEYE_CONTEXT_H_ */
