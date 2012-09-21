#include "game/builders/functions/carrier.h"

#include "game/map/room.h"
#include "game/sprites/worldobject.h"

using std::list;
using sprite::WorldObject;

namespace builder {
namespace function {

void Carrier::operator()(sprite::WorldObject *wobj) {
    list<WorldObject*>::iterator it;
    for(it = drop_list_.begin(); it !=  drop_list_.end(); ++it)
        wobj->current_room()->AddObject(*it, wobj->world_position());
    drop_list_.clear();
    wobj->Die();
}

} // namespace function
} // namespace builder