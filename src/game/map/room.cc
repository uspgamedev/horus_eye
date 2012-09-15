#include "game/map/room.h"

#include <ugdk/graphic/node.h>
#include "game/scenes/world.h"

namespace map {

using std::vector;
using std::list;
using sprite::WorldObject;

void Room::AddToWorld(scene::World* world) {
    for(vector<WorldObject*>::iterator it = walls_.begin(); it != walls_.end(); ++it)
        world->AddWorldObject(*it);
    for(list<WorldObject*>::iterator it = objects_.begin(); it != objects_.end(); ++it)
        world->AddWorldObject(*it);
    world->content_node()->AddChild(floor_);
}

} // namespace map
