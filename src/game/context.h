
#ifndef HORUSEYE_CONTEXT_H_
#define HORUSEYE_CONTEXT_H_

#include <string>

#include <ugdk/math.h>
#include <pyramidworks/collision.h>

#include "game/map.h"
#include "game/sprites/worldobject.h"

namespace context {
    
sprite::WorldObject* WorldObjectByTag (const std::string& tag);

sprite::WorldObject* BuildWorldObject (const std::string& scriptname);

pyramidworks::collision::CollisionObject* MakeCollisionObject (sprite::WorldObject* obj);

void AddCollisionClass (const std::string& classname);

void AddCollisionClass (const std::string& classname, const std::string& supername);

void ActivateRoom(const std::string& roomname);

void DeactivateRoom(const std::string& roomname);

void AddDamageableComponent(const std::string& tag, double life);
void AddDamageableComponent(const map::Room*, const std::string& tag, double life);

} // namespace context


#endif /* HORUSEYE_CONTEXT_H_ */
