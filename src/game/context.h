
#ifndef HORUSEYE_CONTEXT_H_
#define HORUSEYE_CONTEXT_H_

#include <string>

#include <ugdk/math.h>
#include <pyramidworks/collision.h>

#include "game/sprites/worldobject.h"

namespace context {

sprite::WorldObject* BuildWorldObject (const std::string& scriptname);

void AddWorldObject (sprite::WorldObject* new_obj, const ugdk::Vector2D& pos);

void AddWorldObject (const std::string& scriptname, const ugdk::Vector2D& pos);

pyramidworks::collision::CollisionObject* MakeCollisionObject (sprite::WorldObject* obj);

void AddCollisionClass (const std::string& classname);

void AddCollisionClass (const std::string& classname, const std::string& supername);

void ActivateRoom(const std::string& roomname);

} // namespace context


#endif /* HORUSEYE_CONTEXT_H_ */
