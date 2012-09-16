#include "game/builders/functions/carrier.h"

#include "game/scenes/world.h"
#include "game/sprites/worldobject.h"

using std::list;
using scene::World;
using sprite::WorldObject;

namespace builder {
namespace function {

void Carrier::operator()(sprite::WorldObject *wobj) {
    list<WorldObject*>::iterator it;
    scene::World* world = WORLD();
    for(it = drop_list_.begin(); it !=  drop_list_.end(); ++it)
        world->AddWorldObject(*it, wobj->world_position());
    drop_list_.clear();
    wobj->Die();
}

} // namespace function
} // namespace builder