#include "carrier.h"

#include "game/scenes/world.h"

namespace sprite {

void Carrier::Die() {
    if (explosion_ != NULL) {
        WORLD()->AddWorldObject(explosion_, this->world_position());
        explosion_ = NULL;
    }
    Projectile::Die();
}

}
