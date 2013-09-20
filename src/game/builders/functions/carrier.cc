#include "game/builders/functions/carrier.h"

#include "game/map/room.h"
#include "game/sprites/worldobject.h"

using std::list;
using sprite::WorldObject;

namespace builder {
namespace function {

void Carrier::operator()(sprite::WorldObject *wobj) {
    list<WorldObject*>::iterator it;
    for(const auto& drop : drop_list_)
        wobj->current_room()->AddObject(drop, wobj->world_position(), map::POSITION_ABSOLUTE);
    drop_list_.clear();
}

} // namespace function
} // namespace builder