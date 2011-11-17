#include "carrier.h"

#include "game/scenes/world.h"

namespace sprite {

void Carrier::Die() {
    Projectile::Die();
    std::list<WorldObject*>::iterator it;
    for(it = drop_list_.begin(); it !=  drop_list_.end(); ++it)
        WORLD()->AddWorldObject(*it, world_position());
    drop_list_.clear();
}

}
